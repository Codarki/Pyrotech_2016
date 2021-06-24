
static float4 g_light_position_in_view;

texture2D frustum_light_mask_texture;
sampler2D frustum_light_mask_sampler;

void initialize_light(float4 light_position_in_view)
{
    g_light_position_in_view = light_position_in_view;
}

float attenuation(float3 unnormalized_light_direction)
{
    // We drop the light range, to make more physically accurate attenuation.
    //float light_distance = length(light_in_tangent) / light_range;
    float light_distance = length(unnormalized_light_direction) / 100.0f; //50.0f;
    float att = 1.0f / (light_distance * light_distance);
	return att;
}

float3 calculate_light(float3 unnormalized_light_direction, float3 light_color)
{
    float3 I = attenuation(unnormalized_light_direction) * light_color;
    
    //
    float2 light_texcoord = float2(0, 0);
    //Calculate projected light view position        
    light_texcoord.x = g_light_position_in_view.x / g_light_position_in_view.w / 2.0 + 0.5;
    light_texcoord.y = -g_light_position_in_view.y / g_light_position_in_view.w / 2.0 + 0.5;
    
    if (light_texcoord.x >= 0 && light_texcoord.x <= 1 && light_texcoord.y >= 0 && light_texcoord.y <= 1)
    {
        float3 mask = tex2D(frustum_light_mask_sampler, light_texcoord).rgb;
        I *= mask;
    }
    else
    {
        // We are not in light frustum
        I = 0;
    }
    
    return I;
}
