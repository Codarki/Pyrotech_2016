texture2D normal_texture;
sampler2D normal_sampler;

float3 light_color;
float light_range;
float normal_scale;

float fresnel_light_power; // = 16;
float fresnel_view_power; // = 8;

float3 fresnel_view_color; // = {0, 10, 20};
float3 fresnel_light_coeff; // = {10, 10, 10};

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
    
    float N_dot_V = dot(N, V);
    float N_dot_L = dot(N, L);
        
    float fresnel_light = pow(saturate(1 - abs(N_dot_L)), fresnel_light_power);
    float fresnel_view = pow(saturate(1 - abs(N_dot_V)), fresnel_view_power);
        
    color.xyz = fresnel_view * fresnel_view_color;
    color.xyz += fresnel_light * I * fresnel_light_coeff;
                        
    color.w = 1;
}
