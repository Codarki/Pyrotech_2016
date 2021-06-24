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
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    float4 screenPos : TEXCOORD4,
    out float4 color : COLOR0/*,
    out float4 color1 : COLOR1*/)
{
texcoord *= 0.2;

    float2 color_texcoord = texcoord + general_texcoord_offset.xy + color_texcoord_offset.xy;
    float2 emission_texcoord = texcoord + general_texcoord_offset.xy + emission_texcoord_offset.xy;
    float2 normal1_texcoord = color_texcoord;
    float2 normal2_texcoord = color_texcoord * 2;
    float2 normal3_texcoord = color_texcoord * 5;

    float3 I = calculate_light(light_in_world, light_color);
    
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    float3 N1 = tex2D(normal_sampler, normal1_texcoord).xyz * 2 - 1;
    float3 N2 = tex2D(normal_sampler, normal2_texcoord).xyz * 2 - 1;
    float3 N3 = tex2D(normal_sampler, normal3_texcoord).xyz * 2 - 1;
    //color1 = float4(N,1);
	
    N1.z *= normal_scale;
    N2.z *= normal_scale;
    N3.z *= normal_scale;
    N1 = normalize(N1);
    N2 = normalize(N2);
    N3 = normalize(N3);
	
    float3 N = (N1 + N2 + N3) / 3.0;
    //float3 N = N3;
            
    float3 lighting = 0;

    float N_dot_L = saturate(dot(N, L));
    float3 diffuse_coeff = tex2D(diffuse_sampler, color_texcoord).xyz;
    diffuse_coeff = from_srgb_to_linear(diffuse_coeff);
	diffuse_coeff *= diffuse;
    
    if(length(I) * N_dot_L > 0.001)
    {
        // Coefficients for diffuse and specular terms.    
        float3 specular_coeff = tex2D(specular_sampler, color_texcoord).xyz;
        specular_coeff = from_srgb_to_linear(specular_coeff);
        specular_coeff *= specular;
        
        float3 R = -reflect(V, N);
        float3 incoming_reflection = tex2D(reflection_sampler, R.xy + 0.5).xyz;
        incoming_reflection = from_srgb_to_linear(incoming_reflection);
        incoming_reflection *= reflection_coeff;
        
        float3 BRDF = calculate_pixel_BRDF_cook_torrance(N, L, V, diffuse_coeff,
            specular_coeff, roughness_uniform, fresnel_normal_incidence,
            incoming_reflection);
                    
        lighting = BRDF * I * N_dot_L;
    }
    
    
    float3 emitted = tex2D(emission_sampler, emission_texcoord).xyz;
	emitted = from_srgb_to_linear(emitted);
	emitted *= emission_coeff;
    
    //float lighting = 1;
    
    //float3 occ = tex2D(occlusion_sampler, color_texcoord).xyz;

    float2 scUV = (screenPos.xy/screenPos.w);
    scUV.y = -scUV.y;
    scUV/=2;
    scUV +=0.5;
  
    
    float occ = tex2D(occlusion_sampler, scUV).x;
    
   // float3 diffuse_coeff = tex2D(diffuse_sampler, texcoord).xyz * diffuse;
    //float3 occ = calculate_ambient_occlusion(scUV,0.006,0.002,0.007,2.37,position_sampler,occlusion_sampler,random_normal_sampler);
    //float3 occ = calculate_ambient_occlusion2(scUV,0.6,0.5,1.27,0.1,position_sampler,occlusion_sampler,random_normal_sampler);
    
    //color.xyz =  occ * diffuse_coeff;//emitted + (lighting + ambient) * occ;//occ;//diffuse_coeff * occ;//emitted + (lighting + ambient) * occ;
    //color.xyz = lighting;
    //color.xyz = occ;
    color.xyz = emitted + (lighting + ambient) * occ;
                        
    color.w = 1;
    

}
