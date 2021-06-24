
void initialize_light(float4 light_position_in_view)
{
    // Do nothing.
}

float attenuation(float3 unnormalized_light_direction)
{
    return 1;
}

float3 calculate_light(float3 unnormalized_light_direction, float3 light_color)
{
    // No light, return one.
    return 1;
}
