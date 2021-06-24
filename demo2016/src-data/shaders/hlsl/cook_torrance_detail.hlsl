// Lambert diffuse
// Blinn specular
// Ambient occlusion
// Normal mapped
//
texture2D diffuse_texture;
texture2D specular_texture;
texture2D normal_texture;
texture2D occlusion_texture;
texture2D emission_texture;
texture2D reflection_texture;

sampler2D diffuse_sampler;
sampler2D specular_sampler;
sampler2D normal_sampler;
sampler2D occlusion_sampler;
sampler2D emission_sampler;
sampler2D reflection_sampler;

float3 ambient;
float3 diffuse;
float3 specular;
float3 light_color;
float light_range;
float normal_scale;
float roughness_uniform;
float fresnel_normal_incidence;
float reflection_coeff;
float3 emission_coeff;

float3 general_texcoord_offset; // default = 0
float3 color_texcoord_offset; // default = 0
float3 emission_texcoord_offset; // default = 0

void main_ps(
    float2 texcoord : TEXCOORD0,
    float2 texcoord2 : TEXCOORD1,
    float3 view_in_tangent : TEXCOORD2,
    float3 light_in_tangent : TEXCOORD3,
    float3 light_in_world : TEXCOORD4,
    float4 screenPos : TEXCOORD5,
    out float4 color : COLOR0)
{
    float2 color_texcoord = texcoord + general_texcoord_offset.xy + color_texcoord_offset.xy;
    float2 color_texcoord2 = texcoord2 + general_texcoord_offset.xy + color_texcoord_offset.xy;
    float2 emission_texcoord = texcoord2 + general_texcoord_offset.xy + emission_texcoord_offset.xy;

    float3 I = calculate_light(light_in_world, light_color);
    
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    float3 N2 = tex2D(normal_sampler, color_texcoord2).xyz * 2 - 1;
    N2.z *= normal_scale;
    N2 = normalize(N2);

    float3 lighting = 0;

    float N_dot_L = saturate(dot(N2, L));
    
    float3 diffuse_coeff = tex2D(diffuse_sampler, color_texcoord2).xyz;
    diffuse_coeff = from_srgb_to_linear(diffuse_coeff);
	diffuse_coeff *= diffuse;
    
    if(length(I) * N_dot_L > 0.001)
    {
        // Coefficients for diffuse and specular terms.
        float3 specular_coeff = tex2D(specular_sampler, color_texcoord2).xyz;
        specular_coeff = from_srgb_to_linear(specular_coeff);
        specular_coeff *= specular;
        
        float3 R = normalize(-reflect(V, N2));
        float3 incoming_reflection = tex2D(reflection_sampler, R.xy + 0.5).xyz;
        incoming_reflection = from_srgb_to_linear(incoming_reflection);
        incoming_reflection *= reflection_coeff;
        
        float3 BRDF = calculate_pixel_BRDF_cook_torrance(N2, L, V, diffuse_coeff,
            specular_coeff, roughness_uniform, fresnel_normal_incidence,
            incoming_reflection);
                    
        lighting = BRDF * I * N_dot_L;
    }
    
    float3 emitted = tex2D(emission_sampler, emission_texcoord).xyz;
	emitted = from_srgb_to_linear(emitted);
	emitted *= emission_coeff;
	
    float3 occ = tex2D(occlusion_sampler, color_texcoord).xyz;

    float2 coords = (screenPos.xy/screenPos.w);
    coords.y = -coords.y;
    coords/=2;
    coords +=0.5;
    
    occ = 1.0f;//tex2D(occlusion_sampler, coords).xyz;

    
    color.xyz =  emitted + (lighting + ambient) * occ;//occ;//diffuse_coeff * occ;//emitted + (lighting + ambient) * occ;
    //color.xyz = N_dot_L * occ;
                        
    color.w = 1;
}
