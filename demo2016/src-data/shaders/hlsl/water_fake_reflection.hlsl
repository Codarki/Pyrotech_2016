// Water. Fake reflection from texture. "Real" refraction with depth blending.
texture2D diffuse_texture;
texture2D specular_texture;
texture2D normal_texture;
texture2D occlusion_texture;
texture2D emission_texture;
texture2D reflection_texture;
texture2D refraction_texture;
texture2D depth_texture;

sampler2D diffuse_sampler;
sampler2D specular_sampler;
sampler2D normal_sampler;
sampler2D occlusion_sampler;
sampler2D emission_sampler;
sampler2D reflection_sampler;
sampler2D refraction_sampler;
sampler2D depth_sampler;

float3 ambient;
float3 diffuse;
float3 specular;
float3 light_color;
float light_range;
float normal_scale;
float roughness_uniform;
float fresnel_normal_incidence;
float reflection_coeff;
float refraction_coeff;
float3 emission_coeff;

float3 general_texcoord_offset; // default = 0
float3 color_texcoord_offset; // default = 0
float3 emission_texcoord_offset; // default = 0

float3 water_color;
float3 deep_color;
float visibility;
float blend_factor;

float LinearizeDepth(float z)
{
	float n = 1;
	float f = 1600;
    float ndc_z = z * 2.0 - 1.0f;
	return (2 * n) / (f + n - z * (f - n));
}


void main_ps(
    float2 texcoord : TEXCOORD0,
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    float4 screenPos : TEXCOORD4,
    float4 viewPos : BINORMAL,
    float3 light_dir_in_tangent : TEXCOORD5,
    float4 light_view_position : TEXCOORD6,
    float3 normal_in_world : TEXCOORD7,
    out float4 color : COLOR0)
{
    initialize_light(light_view_position);
    float3 I = calculate_light(light_in_world, light_color);
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    
    float3 speed = float3(1.15f, 0.91f, 0.84f);
    float3 scale = float3(0.94f, 0.46f, 1.25f);
    float3 N = water_normal(normal_sampler, texcoord, general_texcoord_offset.xy, normal_scale, speed, scale);
    
    float N_dot_L = saturate(dot(N, L));
    
    float2 scUV = to_screen_space(screenPos);
    
    float fresnel = fresnel_approximation_jari(N, V, fresnel_normal_incidence);
    
    //if(length(I) * N_dot_L > 0.001)
    //{
        // Coefficients for diffuse and specular terms.    
        float3 specular_coeff = tex2D(specular_sampler, texcoord).xyz;
        specular_coeff = from_srgb_to_linear(specular_coeff);
        specular_coeff *= specular;
        
        float3 reflection_color = incoming_reflection_fake(reflection_sampler, N, V, reflection_coeff);        
        
        float3 specular_term = cook_torrance_specular_term(N, L, V, roughness_uniform, fresnel) * specular_coeff; 
         
    //}
            
    float3 N_dot_V =  saturate(dot(N, V));
    float3 refcraction_N = N;
    float3 refract_vector = refract(V, refcraction_N, 1.0 / 1.333);
    float  refract_length = length(refract_vector);
    float2 refraction_uv = (refract_vector).xy * 0.01;

    //float3 lighting = ((specular_term + reflection_color) * fresnel) * I * N_dot_L;
    
    // real refraction
    float depth = (tex2D(depth_sampler, scUV).w);    
    float current_depth = (1.0 - (screenPos.z / screenPos.w));

    
    //float depth_ratio = ((depth / current_depth));
    float3 depthPos = (tex2D(depth_sampler, scUV).xyz);
    float depth_ratio = length(viewPos.xyz - depthPos);
    float depth_y = abs(depthPos.y - viewPos.y);
    

    float3 refraction = tex2D(refraction_sampler,refraction_uv + texcoord * 6).xyz;
    
    float3 diffuse_coeff = from_srgb_to_linear(refraction);
    diffuse_coeff *= diffuse;        
    float3 diffuse_term = oren_nayar_diffuse_term(N, L, V, roughness_uniform) * diffuse_coeff;
    diffuse_term = diffuse_term * I * N_dot_L;
    
    float refr_factor = saturate((depth_ratio) / visibility);
    refraction = lerp(lerp(diffuse_term,water_color * I ,refr_factor),
                 deep_color * I, saturate(depth_y / float3(7.0f, 30.0f, 40.0f)));
    		
    
    
    float3 reflection     = reflection_color * fresnel * I * N_dot_L;
    float3 specular_color = specular_term * I * N_dot_L;
    
    float3 finalCol = lerp(refraction, reflection, fresnel);
    finalCol = saturate(finalCol+specular_color);
    color.xyz = lerp(refraction, finalCol, saturate(depth_ratio * blend_factor));
    
    color.w = 1;//lerp(0,1,saturate(depth_ratio * blend_factor));//lerp(0,1,saturate(depth_ratio / (visibility * 2)));//max(depth_ratio, fresnel);
}
