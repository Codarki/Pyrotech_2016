//
// Relief mapping
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
float specular_power;
float specular_strength;
float3 light_color;
float light_range;
float normal_scale;
float roughness_uniform;
float fresnel_normal_incidence;
float depth_uniform;

float reflection_coeff;
float3 emission_coeff;

float3 general_texcoord_offset; // default = 0
float3 color_texcoord_offset; // default = 0
float3 emission_texcoord_offset; // default = 0

//float tile  = 1.0;
//static float depth = -0.02; // 0.0376;

// ray intersect depth map using linear and binary searches
// depth value stored in alpha channel (black at is object surface)
void ray_intersect_relief(inout float3 position_in_texture,
    float3 view_direction, sampler2D relief_map, float V_dot_geom_N)
{    
    const int linear_step_count = 10;
    const int binary_step_count = 5;
        
    float3 trace_direction = view_direction;
    //trace_direction.xy *= depth_uniform;
    
    // Depth bias correction. Is this really required?
    //float tmp_z = abs(view_direction.z);
    //float depth_bias = 1.0 - pow(1.0 - tmp_z, 8);
    //trace_direction.xy *= depth_bias;
    
    trace_direction.xy *= depth_uniform * (2 * trace_direction.z - trace_direction.z * trace_direction.z);
    trace_direction /= trace_direction.z;
        
    float trace_length = 1.0 / (view_direction.z * linear_step_count);

    //const int max_steps_lin = 10;
    //const int max_steps_bin = 5;
    //int num_steps_lin = max_steps_lin * saturate(1 - V_dot_geom_N) + 1;
    //int num_steps_bin = max_steps_bin * saturate(1 - V_dot_geom_N);

    for(int i = 0; i < linear_step_count; ++i)
    {
        float height = tex2D(relief_map, position_in_texture.xy).w;
        
        if(position_in_texture.z < height)
            position_in_texture += trace_direction * trace_length;
    }
    
    // we have already intersected inside, trace back half step so binary
    // search doesnt sample same position.
    trace_length *= 0.5;
    position_in_texture -= trace_direction * trace_length;

    for(int i = 0; i < binary_step_count; ++i)
    {
        float height = tex2D(relief_map, position_in_texture.xy).w;
        float height_difference = height - position_in_texture.z;

        trace_length *= 0.5;
    
        //position_in_texture += trace_direction * trace_length * sign(height_difference);
        
        // Trace forward one step. If we were inside height map then take two
        // step back. Same function as line above, dont know which is faster.
        float3 position_offset = trace_direction * trace_length;
        position_in_texture += position_offset;
        if(height_difference < 0)
            position_in_texture -= position_offset * 2;
    }
}

void main_ps(
    float4 position : POSITION,
    float2 texcoord : TEXCOORD0,
    float3 light_in_tangent : TEXCOORD1,
    float3 view_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    out float4 color : COLOR0)
{
    float3 I = calculate_light(light_in_world, light_color);

    float2 color_texcoord = texcoord + general_texcoord_offset.xy + color_texcoord_offset.xy;
    
    //float V_dot_geom_N = dot(normalize(normal_in_tangent), normalize(view_in_tangent));

    float3 position_in_texture = float3(color_texcoord, 0);
    float3 view_dir_in_tangent = normalize(view_in_tangent);
        
    ray_intersect_relief(position_in_texture.xyz, view_dir_in_tangent, normal_sampler, 0);
        
    color_texcoord = position_in_texture.xy;
        
    float3 L = normalize(light_in_tangent);
    float3 V = normalize(view_in_tangent);
    float3 N = tex2D(normal_sampler, color_texcoord).xyz * 2 - 1;
    //N.z *= normal_scale;
    N = normalize(N);
    
   float3 lighting = 0;
 
    float N_dot_L = saturate(dot(N, L));
    
    if(length(I) * N_dot_L > 0.001)
    {
        // Coefficients for diffuse and specular terms.    
        float3 specular_coeff = tex2D(specular_sampler, color_texcoord).xyz;
        specular_coeff = from_srgb_to_linear(specular_coeff);
        specular_coeff *= specular;
		
		float3 diffuse_coeff = tex2D(diffuse_sampler, color_texcoord).xyz;
		diffuse_coeff = from_srgb_to_linear(diffuse_coeff);
		diffuse_coeff *= diffuse;
        
        float3 R = -reflect(V, N);
        float3 incoming_reflection = tex2D(reflection_sampler, R.xy + 0.5).xyz;
        incoming_reflection = from_srgb_to_linear(incoming_reflection);
        incoming_reflection *= reflection_coeff;
        
        float3 BRDF = calculate_pixel_BRDF_cook_torrance(N, L, V, diffuse_coeff,
            specular_coeff, roughness_uniform, fresnel_normal_incidence,
            incoming_reflection);
                    
        lighting = BRDF * I * N_dot_L;
    }
    
  /*
    float3 diffuse_coeff = tex2D(diffuse_sampler, color_texcoord).xyz * diffuse;
    float3 specular_coeff = tex2D(specular_sampler, color_texcoord).xyz * specular;
    //float3 occ = tex2D(occlusion_sampler, color_texcoord).xyz;
    
    float3 incoming_reflection = 1;
    
    float3 BRDF = calculate_pixel_BRDF_cook_torrance(N, L, V, diffuse_coeff,
        specular_coeff, roughness_uniform, fresnel_normal_incidence,
        incoming_reflection);
    float N_dot_L = saturate(dot(N, L));
    lighting = BRDF * I * N_dot_L;
*/
    float2 emission_texcoord = color_texcoord + general_texcoord_offset.xy + emission_texcoord_offset.xy;
    float3 emitted = tex2D(emission_sampler, emission_texcoord).xyz;
	emitted = from_srgb_to_linear(emitted);
	emitted *= emission_coeff;
    
    color.xyz = (ambient+lighting);
    color.xyz += (emitted );
                
    color.w = 1;
}