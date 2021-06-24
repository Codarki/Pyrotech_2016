//#include "common_ps.hlsl"

float3 incoming_reflection(sampler2D reflection_sampler, float4 view_clip_position, float3 N)
{
    float2 screen_position = view_clip_position.xy / view_clip_position.w;
    float2 reflection_texcoord = 0.5f * float2(screen_position.x, -screen_position.y) + 0.5f;
    
    reflection_texcoord += N.xy * 0.02f;
    
    float3 reflection_color = tex2D(reflection_sampler, reflection_texcoord).xyz;
    return reflection_color;
}

float3 incoming_reflection_fake(sampler2D reflection_sampler, float3 N, float3 V, float reflection_coeff)
{
    float3 R = -reflect(V, N);
    float3 reflection_color = tex2D(reflection_sampler, R.xy + 0.5).xyz;
    reflection_color = from_srgb_to_linear(reflection_color);
    reflection_color *= reflection_coeff;
    return reflection_color;
}

float3 water_normal(sampler2D normal_sampler, float2 texcoord, float2 general_texcoord_offset, float normal_scale, float3 speed, float3 scale)
{
	float angle0 = 0.3;
	float angle1 = -0.3;
	float2x2 rotation0 = {cos(angle0), -sin(angle0), sin(angle0), cos(angle0)};
	float2x2 rotation1 = {cos(angle1), -sin(angle1), sin(angle1), cos(angle1)};
	
	float2 direction0 = general_texcoord_offset;
	float2 direction1 = mul(rotation0, direction0);
	float2 direction2 = mul(rotation1, direction0);
    float2 normal1_texcoord = texcoord + direction0 * speed.x;
    float2 normal2_texcoord = texcoord + direction1 * speed.y;
    float2 normal3_texcoord = texcoord + direction2 * speed.z;
    float3 N1 = (tex2D(normal_sampler, normal1_texcoord).xyz * 2 - 1) * scale.x;
    float3 N2 = (tex2D(normal_sampler, normal2_texcoord).xyz * 2 - 1) * scale.y;
    float3 N3 = (tex2D(normal_sampler, normal3_texcoord).xyz * 2 - 1) * scale.z;
    N1.z *= normal_scale;
    N2.z *= normal_scale;
    N3.z *= normal_scale;
    // N1 = normalize(N1);
    // N2 = normalize(N2);
    // N3 = normalize(N3);
    float3 N = normalize((N1 + N2 + N3) * 0.33f);
    return N;
}

