texture2D specular_texture;
texture2D normal_texture;
texture2D reflection_texture;
texture2D depth_texture;

sampler2D specular_sampler;
sampler2D normal_sampler;
sampler2D reflection_sampler;
sampler2D depth_sampler;

float3 specular;
float3 light_color;
float normal_scale;
float specular_exponent;
float reflection_coeff;
float fresnel_normal_incidence;
float visibility;

float3 general_texcoord_offset; // default = 0

float3 surface_color;
float3 depth_color;
float3 speed;// = float3(1.15f, 0.91f, 0.84f);
float3 scale;// = float3(0.94f, 0.46f, 1.25f);

void main_ps(
    float2 texcoord : TEXCOORD0,
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    float4 screenPos : TEXCOORD4,
    float3 light_dir_in_tangent : TEXCOORD5,
    float4 light_view_position : TEXCOORD6,
    float3 normal_in_world : TEXCOORD7,
    float4 viewPos : BINORMAL,
    out float4 color : COLOR0)
{
    initialize_light(light_view_position);
    float3 I = calculate_light(light_in_world, light_color);
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    
    float3 N = water_normal(normal_sampler, texcoord, general_texcoord_offset.xy, normal_scale, speed, scale);
    
    float N_dot_L = saturate(dot(N, L));
    
    float2 scUV = to_screen_space(screenPos);

    float3 reflection_color = incoming_reflection(reflection_sampler, screenPos, N);
    reflection_color *= reflection_coeff;

    float fresnel = fresnel_approximation_jari(N, V, fresnel_normal_incidence);
    
    //if(length(I) * N_dot_L > 0.001)
    //{
        // Coefficients for diffuse and specular terms.    
        float3 specular_coeff = tex2D(specular_sampler, texcoord).xyz;
        specular_coeff = from_srgb_to_linear(specular_coeff);
        specular_coeff *= specular;
        
        //float3 specular_coeff = specular;
        float3 specular_term = blinn_phong_specular_term(N, L, V, specular_coeff, specular_exponent) * I;
                
        //float3 specular_term = cook_torrance_specular_term(N, L, V, roughness_uniform, fresnel) * specular_coeff;            
    //}
            		    
    float3 depthPos = (tex2D(depth_sampler, scUV).xyz);
    float depth_distance = length(viewPos.xyz - depthPos);
	
	depth_distance = depth_distance / visibility;
	float water_attenuation = 1.0f / (depth_distance * depth_distance);
	
	float water_alpha = saturate(1 - water_attenuation);
	
	float3 water_color = lerp(surface_color, depth_color, water_alpha);
	//float3 water_color = lerp(surface_color, depth_color, saturate(depth_distance));
        
    float3 illuminated_water_color = water_color * water_alpha * I;
    float3 surface_color2 = lerp(illuminated_water_color, reflection_color, fresnel);
	
	float3 diffuse_term = N_dot_L * specular * I;
    
    color.xyz = surface_color2 + specular_term + diffuse_term;
    //color.xyz = diffuse_term;
    //color.xyz = float3(1, 0, 0);
    
    color.w = max(water_alpha, fresnel);
    //color.w = fresnel;
}
