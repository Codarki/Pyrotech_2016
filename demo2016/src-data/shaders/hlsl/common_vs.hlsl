
// Transform position from homogeneus coordinates.
float3 dehomo(float4 position_h)
{
    return position_h.xyz / position_h.w;
}

// Transform position to homogeneus coordinates.
float4 homogeneus_position(float3 position)
{
    return float4(position.xyz, 1);
}

struct vectors_in_object
{
    float3 position;
    float3 view_direction;
    float3 light_direction;
    float3 directional_light_direction;
};

// Calculates non-homogeneus position, light and view vectors.
// light_dir_in_world is for directional and spot lights
vectors_in_object calculate_vectors_in_object(
    float4x4 world_to_object,
    float4 position_h, float3 view_in_world, float3 light_in_world, float3 light_dir_in_world)
{
    vectors_in_object result;

    float3 position_in_object = dehomo(position_h);
    float3 view_in_object = dehomo(mul(world_to_object, homogeneus_position(view_in_world)));
    float3 light_in_object = dehomo(mul(world_to_object, homogeneus_position(light_in_world)));
    float3 light_dir_in_object = dehomo(mul(world_to_object, homogeneus_position(light_dir_in_world)));

    //float3 view_in_object = mul(world_to_object, float4(view_in_world, 1)).xyz;
    //float3 light_in_object = mul(world_to_object, float4(light_in_world, 1)).xyz;
 
    result.position = position_in_object;
    result.view_direction = view_in_object - position_in_object;
    result.light_direction = light_in_object - position_in_object;
    result.directional_light_direction = light_dir_in_object - light_in_object; // - position_in_object;
    return result;
}

float3x3 calculate_object_to_tangent_orthonormal(
    float3 tangent,
    float3 normal,
    float handedness)
{
    // Calculate tangent and bitangent.
    float3 T = tangent;
    float3 B = cross(normal, tangent) * handedness;
    float3 N = normal;
    
    // Gram-Schmidt orthogonalize
    T = T - dot(N, T) * N;
    B = B - dot(N, B) * N - dot(T, B) * T;
    
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
    
    return float3x3(T, B, N);
}

float3x3 calculate_object_to_tangent_orthogonal(
    float3 tangent,
    float3 normal,
    float handedness)
{
    // Calculate tangent and bitangent.
    float3 T = tangent;
    float3 B = cross(normal, tangent) * handedness;
    float3 N = normal;
    
    // Gram-Schmidt orthogonalize
    T = T - dot(N, T) * N;
    B = B - dot(N, B) * N - dot(T, B) * T;
        
    return float3x3(T, B, N);
}

float3x3 calculate_object_to_tangent(
    float3 tangent,
    float3 normal,
    float handedness)
{
    // Calculate tangent and bitangent.
    float3 T = tangent;
    float3 B = cross(normal, tangent) * handedness;
    float3 N = normal;
            
    return float3x3(T, B, N);
}

void distort_position(
    inout float4 position, float3 time, float3 frequency, float3 scale,
    float3 pos_scale)
{
    float3 pos = position.xyz / position.w;
    
    float3 position_offset = 0;
    position_offset.x = sin((pos.y * pos_scale.x + time.x) * frequency.x) * scale.x;
    position_offset.y = sin((pos.x * pos_scale.x + time.y) * frequency.y) * scale.y;
    position_offset.z = sin((pos.z * pos_scale.x - time.z) * frequency.z) * scale.z;
    
    position = float4(pos + position_offset, 1);
}
