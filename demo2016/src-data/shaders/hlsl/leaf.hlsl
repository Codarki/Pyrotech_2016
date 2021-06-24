// cook_torrance_alpha_tested_double_sided + subsurface scattering fake

texture2D diffuse_texture;
texture2D specular_texture;
texture2D shininess_texture;
texture2D normal_texture;
texture2D occlusion_texture;
texture2D emission_texture;
texture2D reflection_texture;
texture2D alpha_texture;
texture2D scatter_texture;

sampler2D diffuse_sampler;
sampler2D specular_sampler;
sampler2D shininess_sampler;
sampler2D normal_sampler;
sampler2D occlusion_sampler;
sampler2D emission_sampler;
sampler2D reflection_sampler;
sampler2D alpha_sampler;
sampler2D scatter_sampler;

float3 ambient;
float3 diffuse;
float3 specular;
float3 scatter_color;
float3 light_color;
float light_range;
float normal_scale;
float roughness_uniform;
float diffuse_roughness_uniform;
float4 diffuse_specular_shininess_minmax_uniform;
float fresnel_normal_incidence;
float reflection_coeff;
float3 emission_coeff;

float3 general_texcoord_offset; // default = 0
float3 color_texcoord_offset; // default = 0
float3 emission_texcoord_offset; // default = 0

void main_ps(
    float facing : VFACE,
    float2 texcoord : TEXCOORD0,
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    float4 screenPos : TEXCOORD4,
    float3 light_dir_in_tangent : TEXCOORD5,
    float4 light_view_position : TEXCOORD6,
    float3 normal_in_world : TEXCOORD7,
    out float4 color : COLOR0/*,
    out float4 color1 : COLOR1*/)
{
	float4 alpha_texel = tex2D(alpha_sampler, texcoord).xyzw;
	//float alpha_coeff_from_alpha_map = dot(alpha_texel.xyz, float3(1, 1, 1)) / 3.0f;
	float alpha_coeff_from_alpha_map = alpha_texel.w;
    
    clip(alpha_coeff_from_alpha_map < 0.5f ? -1 : 1 );
    
    float2 color_texcoord = texcoord + general_texcoord_offset.xy + color_texcoord_offset.xy;
    float2 emission_texcoord = texcoord + general_texcoord_offset.xy + emission_texcoord_offset.xy;
    
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    float3 N = tex2D(normal_sampler, color_texcoord).xyz * 2 - 1;
    
    initialize_light(light_view_position);

    float3 I = calculate_light(light_in_world, light_color);
  
    N *= facing;
    //color1 = float4(N,1);
    N.z *= normal_scale;
    N = normalize(N);
    //N = float3(0, 0, 1);
            
    float3 lighting = 0;

    float N_dot_L = saturate(dot(N, L));
    float3 diffuse_coeff = tex2D(diffuse_sampler, color_texcoord).xyz;
    diffuse_coeff = from_srgb_to_linear(diffuse_coeff);
    diffuse_coeff *= diffuse;
    
    //if(length(I) * N_dot_L > 0.001)
    {
        // Coefficients for diffuse and specular terms.    
        float3 specular_coeff = tex2D(specular_sampler, color_texcoord).xyz;
        specular_coeff = from_srgb_to_linear(specular_coeff);
        specular_coeff *= specular;
        
        float3 shininess_texel = tex2D(shininess_sampler, color_texcoord).xyz;
        float diffuse_shininess_coeff = lerp(diffuse_specular_shininess_minmax_uniform.x, diffuse_specular_shininess_minmax_uniform.y, shininess_texel.x);
        float specular_shininess_coeff = lerp(diffuse_specular_shininess_minmax_uniform.z, diffuse_specular_shininess_minmax_uniform.w, shininess_texel.y);
        float diffuse_roughness_coeff = 1 - diffuse_shininess_coeff;
        float specular_roughness_coeff = 1 - specular_shininess_coeff;
        
        //float3 roughness_coeff = 1 - shininess_coeff;
        // roughness_coeff = from_srgb_to_linear(roughness_coeff); no srgb conversion, should be calculated.
        
        float3 R = -reflect(V, N);
        float3 incoming_reflection = tex2D(reflection_sampler, R.xy + 0.5).xyz;
        incoming_reflection = from_srgb_to_linear(incoming_reflection);
        incoming_reflection *= reflection_coeff;
        incoming_reflection *= specular_shininess_coeff;
        
        float diffuse_roughness = diffuse_roughness_coeff * diffuse_roughness_uniform;
        float specular_roughness = specular_roughness_coeff * roughness_uniform;
        float fresnel_normal_incidence2 = fresnel_normal_incidence ;//* diffuse_shininess_coeff;
        
        float3 BRDF = calculate_pixel_BRDF_cook_torrance(N, L, V,
            diffuse_coeff,
            specular_coeff,
            specular_roughness,
            diffuse_roughness,
            fresnel_normal_incidence2,
            incoming_reflection);
                    
        lighting = BRDF  * I * N_dot_L;
    }
    
    float3 emitted = tex2D(emission_sampler, emission_texcoord).xyz;
    emitted = from_srgb_to_linear(emitted);
    emitted *= emission_coeff;
        
    //float3 occ = tex2D(occlusion_sampler, color_texcoord).xyz;

    float2 scUV = (screenPos.xy/screenPos.w);
    scUV.y = -scUV.y;
    scUV/=2.0;
    scUV +=0.5;
    
    scUV.y += 0.007;
    float3 occ = tex2D(occlusion_sampler, scUV).xyz;
    
    float fresnel_tmp = fresnel_approximation_schlick(L, V, fresnel_normal_incidence);
    float3 scattering_texel = tex2D(scatter_sampler, texcoord).xyz;
    
    scattering_texel *= I * scatter_color;
    scattering_texel *= 0.1; // hack coeff
    
    //scattering_texel *= (1 - fresnel_tmp);
    //scattering_texel *= (-facing);
    //color.xyz = scattering_texel * fresnel_tmp;
    
    color.xyz = emitted + lighting * saturate(occ) + scattering_texel;
    //color.xyz = emitted + lighting * saturate(occ);
    color.w = 1 * alpha_coeff_from_alpha_map;
}
