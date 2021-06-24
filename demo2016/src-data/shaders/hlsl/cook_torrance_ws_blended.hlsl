// Lambert diffuse
// Blinn specular
// Ambient occlusion
// Normal mapped
//
texture2D diffuse_texture1;
texture2D diffuse_texture2;
texture2D specular_texture1;
texture2D specular_texture2;
texture2D normal_texture1;
texture2D normal_texture2;
texture2D occlusion_texture;
texture2D emission_texture1;
texture2D emission_texture2;
texture2D reflection_texture1;
texture2D reflection_texture2;

sampler2D diffuse_sampler1;
sampler2D diffuse_sampler2;
sampler2D specular_sampler1;
sampler2D specular_sampler2;
sampler2D normal_sampler1;
sampler2D normal_sampler2;
sampler2D occlusion_sampler;
sampler2D emission_sampler1;
sampler2D emission_sampler2;
sampler2D reflection_sampler1;
sampler2D reflection_sampler2;

float3 ambient;
float3 diffuse1;
float3 diffuse2;
float3 specular1;
float3 specular2;
float3 light_color;
float light_range;
float normal_scale1;
float normal_scale2;
float roughness_uniform;
float diffuse_roughness_uniform;
float fresnel_normal_incidence;
float reflection_coeff;
float3 emission_coeff1;
float3 emission_coeff2;

float3 general_texcoord_offset; // default = 0
float3 color_texcoord_offset; // default = 0
float3 emission_texcoord_offset; // default = 0

void main_ps(
    float2 texcoord : TEXCOORD0,
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    float4 screenPos : TEXCOORD4,
	float4 position_in_world : TEXCOORD5,
    out float4 color : COLOR0/*,
    out float4 color1 : COLOR1*/)
{
//texcoord *= 0.2;
    float blend_coeff = 0.5;
	//float screenpos_distance = length(screenPos.xy / screenPos.w);
	//blend_coeff = saturate(-0.2 + screenpos_distance);
	
	float3 pos_in_world = position_in_world.xyz / position_in_world.w;
	blend_coeff = saturate(pos_in_world.z * 0.2);
	
	//float2 screenpos_distance = screenPos.xy / screenPos.w;
    //screenpos_distance.y = -screenpos_distance.y;
    //screenpos_distance /= 2.0;
    //screenpos_distance += 0.5;
	//blend_coeff = length(screenpos_distance);

    float2 color_texcoord = texcoord + general_texcoord_offset.xy + color_texcoord_offset.xy;
    float2 emission_texcoord = texcoord + general_texcoord_offset.xy + emission_texcoord_offset.xy;

    float3 I = calculate_light(light_in_world, light_color);
    
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
	
    float3 N1 = tex2D(normal_sampler1, color_texcoord).xyz * 2 - 1;
    float3 N2 = tex2D(normal_sampler2, color_texcoord).xyz * 2 - 1;
	
    //color1 = float4(N,1);
    N1.z *= normal_scale1;
    N2.z *= normal_scale2;
	
	float3 N = lerp(N1, N2, blend_coeff);
	
    N = normalize(N);
    //N = float3(0, 0, 1);
            
    float3 lighting = 0;

    float N_dot_L = saturate(dot(N, L));
    float3 diffuse_coeff1 = tex2D(diffuse_sampler1, color_texcoord).xyz;
    float3 diffuse_coeff2 = tex2D(diffuse_sampler2, color_texcoord).xyz;
    diffuse_coeff1 = from_srgb_to_linear(diffuse_coeff1);
    diffuse_coeff2 = from_srgb_to_linear(diffuse_coeff2);
	diffuse_coeff1 *= diffuse1;
	diffuse_coeff2 *= diffuse2;
	float3 diffuse_coeff = lerp(diffuse_coeff1, diffuse_coeff2, blend_coeff);
    
    //if(length(I) * N_dot_L > 0.001)
    {
        // Coefficients for diffuse and specular terms.    
        float3 specular_coeff1 = tex2D(specular_sampler1, color_texcoord).xyz;
        float3 specular_coeff2 = tex2D(specular_sampler2, color_texcoord).xyz;
        specular_coeff1 = from_srgb_to_linear(specular_coeff1);
        specular_coeff2 = from_srgb_to_linear(specular_coeff2);
        specular_coeff1 *= specular1;
        specular_coeff2 *= specular2;
		float3 specular_coeff = lerp(specular_coeff1, specular_coeff2, blend_coeff);
        
        float3 R = -reflect(V, N);
        float3 incoming_reflection1 = tex2D(reflection_sampler1, R.xy + 0.5).xyz;
        float3 incoming_reflection2 = tex2D(reflection_sampler2, R.xy + 0.5).xyz;
		float3 incoming_reflection = lerp(incoming_reflection1, incoming_reflection2, blend_coeff);
        incoming_reflection = from_srgb_to_linear(incoming_reflection);
        incoming_reflection *= reflection_coeff;
        
        float3 BRDF = calculate_pixel_BRDF_cook_torrance(N, L, V, diffuse_coeff,
            specular_coeff,
			roughness_uniform,
			diffuse_roughness_uniform,
			fresnel_normal_incidence,
            incoming_reflection);
                    
        lighting = BRDF * I * N_dot_L;
    }
    
    float3 emitted1 = tex2D(emission_sampler1, emission_texcoord).xyz;
	emitted1 = from_srgb_to_linear(emitted1);
	emitted1 *= emission_coeff1;

    float3 emitted2 = tex2D(emission_sampler2, emission_texcoord).xyz;
	emitted2 = from_srgb_to_linear(emitted2);
	emitted2 *= emission_coeff2;

	float3 emitted = lerp(emitted1, emitted2, blend_coeff);
    
    //float lighting = 1;
    //float3 occ = tex2D(occlusion_sampler, color_texcoord).xyz;

    float2 scUV = (screenPos.xy/screenPos.w);
    scUV.y = -scUV.y;
    scUV/=2.0;
    scUV +=0.5;

    
    
  scUV.y += 0.007;
float3 occ = tex2D(occlusion_sampler, scUV).xyz;

    color.xyz = emitted + (lighting + ambient) * saturate(occ);
    color.w = 1;
}
