//
// Tangent-Bitangent-Normal vertex shader
//

float4x4 object_to_world;
float4x4 world_to_object;
float4x4 world_to_view;
float4x4 projection;

// View position in world space
float3 view_in_world;
float3 light_position_in_world;

struct input_vertex
{
    float4 position : POSITION;
    float4 tangent : TANGENT;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct output_vertex
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 view_in_tangent : TEXCOORD1;
    float3 light_in_tangent : TEXCOORD2;
    float3 light_in_world : TEXCOORD3;
    float4 screenPos : TEXCOORD4;
};

void main_vs(in input_vertex input, out output_vertex output)
{
    float4 position_in_world = input.position;

	float4x4 world_to_view_orientation = world_to_view;
	world_to_view_orientation[3] = float4(0, 0, 0, 1);

    float4 position_in_view = mul(world_to_view_orientation, position_in_world);
    output.position = mul(projection, position_in_view);
    
    vectors_in_object in_object = calculate_vectors_in_object(
        world_to_object, input.position, view_in_world,
        light_position_in_world);

    float3x3 object_to_tangent = calculate_object_to_tangent_orthonormal(
        input.tangent.xyz, input.normal, input.tangent.w);
        
    //output.tangent = object_to_tangent[0];
    //output.bitangent = object_to_tangent[1];
    //output.normal = object_to_tangent[2];
    
    output.texcoord = input.texcoord;
    output.view_in_tangent = mul(object_to_tangent, in_object.view_direction);
    output.light_in_tangent = mul(object_to_tangent, in_object.light_direction);
    
    // For attenuation, to avoid non-uniform scaling in light_in_tangent.
    output.light_in_world = position_in_world - light_position_in_world;
    
    //For occlusion
    output.screenPos = output.position;    
}
