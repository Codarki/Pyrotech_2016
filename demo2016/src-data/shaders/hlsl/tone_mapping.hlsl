// Vertex shader

struct input_vertex
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

void main_vs(inout input_vertex input)
{
};

// Pixel shader

//#include "tone_mapping_functions.hlsl"
        
texture2D source_texture;
texture2D add_texture;
sampler2D source_sampler;
sampler2D s1;

// Default 0.18
float middle_gray;

// 
float adapted_luminance;
float saturation; // 0 is black and white, 1 is normal;
float3 color_correction;
float3 negation_color;
float4 attribute1; // used for yuv

// Denotes the smallest luminance value that will be mapped to white.
// For low-dynamic-range images, setting White to smaller values yields a subtle contrast enhancement
// 1.0 might be decent.
float white_point;

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    float3 original_color = tex2D(source_sampler, texcoord).xyz;
	
	// Clamp negatives.
	original_color = max(0, original_color);
	
	//original_color.xyz *= color_correction;
	//original_color.xyz += color_correction * 0.00001;
	
	//original_color.xyz *= pow(abs(original_color), color_correction);
		
    color.xyz = Reinhard_ToneMap(original_color, adapted_luminance, middle_gray, white_point);
	    
    // Add bloom.
    //color.xyz += tex2D(s1, texcoord).xyz;
	
    // Blend with bloom.
	float3 bloom_color = saturate(tex2D(s1, texcoord).xyz);
	//float bloom_luminance = dot(bloom_color, float3(0.2126, 0.7152, 0.0722));
	//color.xyz = lerp(color.xyz, bloom_color, bloom_luminance);
	color.xyz = lerp(color.xyz, bloom_color, bloom_color);
	
	// Contrast color. color must be in range 0 - 1
	float3 contrast = color_correction;
	float3 saturated_color = saturate(color.xyz);
	color.xyz = saturated_color.xyz - contrast * (saturated_color.xyz - 1.0f) * saturated_color.xyz * (saturated_color.xyz - 0.5f);
	
	color.xyz = saturate(color.xyz);

    // Color saturation
	float lum = dot(color.xyz, float3(0.2126, 0.7152, 0.0722));
	color.xyz = lerp(lum.xxx, color.xyz, saturation);
	
	// convert from linear gamma to gamma 2.2
	color.xyz = pow(abs(saturate(color.xyz)), 1.0 / 2.2);
	    
    color.xyz = abs(negation_color - color.xyz);
    
    float3 yuv = lerp(color.xyz,rgb_to_yuv(color.xyz),float3(attribute1.rgb));
    color.xyz = yuv;
    
    color.w = 1;
}
