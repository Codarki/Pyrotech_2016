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
sampler2D source_sampler;
float middle_gray;
float adapted_luminance;

// Threshold in the bright pass tone mapping operator moves the whole curve
// into the -y direction.
float threshold;

// Increasing the value of offset makes the curve steeper, which means it is
// more sensitive to light changes and decreasing makes it more shallow, so
// that it is less sensitive to light changes.
float offset;

// Denotes the smallest luminance value that will be mapped to white.
// For low-dynamic-range images, setting White to smaller values yields a subtle contrast enhancement
// 10.0 might be decent for bright pass.
float white_point;

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    float3 original_color = tex2D(source_sampler, texcoord).xyz;
	
	// Clamp negatives.
	original_color = max(0, original_color);
	
    color.xyz = Reinhard_Brightpass(original_color, adapted_luminance, middle_gray, white_point, threshold, offset);

    /*
    float3 original_color = tex2D(source_sampler, texcoord).xyz;
    original_color *= middle_gray / (adapted_luminance + 0.001f);
    original_color -= threshold;
    original_color = max(original_color, 0);
    original_color /= (offset + original_color);
    color.xyz = original_color;
    */
    
    color.w = 1;
}
