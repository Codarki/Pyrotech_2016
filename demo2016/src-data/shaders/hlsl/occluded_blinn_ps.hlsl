// Lambert diffuse
// Blinn specular
// Ambient occlusion
// Normal mapped
//
texture2D diffuse_texture;
texture2D specular_texture;
texture2D normal_texture;
texture2D occlusion_texture;

sampler2D diffuse_sampler;
sampler2D specular_sampler;
sampler2D normal_sampler;
sampler2D occlusion_sampler;

float3 diffuse;
float3 specular;
float specular_power;
float specular_strength;
float3 light_color;
float light_range;
float normal_scale;
	
void main_ps(
    float2 texcoord : TEXCOORD0,
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    out float4 color : COLOR0)
{
    float3 I = calculate_light(light_in_world, light_color);

    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    float3 N = tex2D(normal_sampler, texcoord).xyz * 2 - 1;
    N.z *= normal_scale;
    N = normalize(N);
    
    // Coefficients for diffuse and specular terms.
    float3 diffuse_coeff = tex2D(diffuse_sampler, texcoord).xyz;
    diffuse_coeff = from_srgb_to_linear(diffuse_coeff);
	diffuse_coeff *= diffuse;
	
	float3 specular_coeff = tex2D(specular_sampler, texcoord).xyz;
	specular_coeff = from_srgb_to_linear(specular_coeff);
	specular_coeff *= specular;
	
    float3 occ = tex2D(occlusion_sampler, texcoord).xyz;
    
    color.xyz = calculate_pixel_color_blinn(N, L, V, diffuse_coeff, specular_coeff, specular_power, I);
    color.xyz *= occ;
                    
    color.w = 1;
}
