
void initialize_light(float4 light_position_in_view)
{
    // Do nothing.
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
    return I;
}
