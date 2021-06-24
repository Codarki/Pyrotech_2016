//
// Relief mapping VS
//

float4x4 object_to_world;
float4x4 world_to_object;
float4x4 world_to_view;
float4x4 world_to_view_clip;
float4x4 projection;
float3 view_in_world;
float3 light_position_in_world;

float fHeightMapRange = 0.02;
int   fTile = 1;

float3 distort_time_uniform;
float3 distort_frequency_uniform;
float3 distort_scale_uniform;

struct input_vertex
{
    float4 position : POSITION;
    float4 tangent  : TANGENT;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct output_vertex
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 light_in_tangent : TEXCOORD1;
    float3 view_in_tangent : TEXCOORD2;
    float3 light_in_world : TEXCOORD3;
};

void main_vs(in input_vertex input, out output_vertex output)
{
    float4 position_in_world = mul(object_to_world, input.position);
    
    distort_position(position_in_world,
        distort_time_uniform,
        distort_frequency_uniform,
        distort_scale_uniform * 1.0, 1.0);

    float4 position_in_view = mul(world_to_view, position_in_world);
    
    // View space distort begins.
    // We distort with another spherical coordinate.
    float atan_view_xy = atan2(position_in_view.y, position_in_view.x);
    float sin_atan_view_xy
        = sin(atan_view_xy * 2 + distort_time_uniform.x) * 0.5
        + sin(atan_view_xy * 4 + distort_time_uniform.y) * 0.25
        + sin(atan_view_xy * 8 + distort_time_uniform.z) * 0.25;
         
    // Dont distort anything in front.
    float atan_view_zy = 1 - abs(atan2(position_in_view.y, position_in_view.z) / 3.1415);
    float view_distort_scale = 1 - pow(atan_view_zy, 6);
    
    // Also scale down distortion of vertices near camera, the angle changes so much faster up close.
    view_distort_scale *= position_in_view.z / 40.0;

    position_in_view.xy += normalize(position_in_view.xy) * sin_atan_view_xy * view_distort_scale * 8;
    // View space distort ends.
        
    output.position = mul(projection, position_in_view);
    
    vectors_in_object in_object = calculate_vectors_in_object(
        world_to_object, input.position, view_in_world,
        light_position_in_world);

    float3x3 object_to_tangent = calculate_object_to_tangent(
        input.tangent.xyz, input.normal, input.tangent.w);

    output.texcoord = input.texcoord;

    output.view_in_tangent = mul(object_to_tangent, in_object.view_direction);
    output.light_in_tangent = mul(object_to_tangent, in_object.light_direction);
    
    // For attenuation, to avoid non-uniform scaling in light_in_tangent.
    output.light_in_world = position_in_world - light_position_in_world;
}
