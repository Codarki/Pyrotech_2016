//
// Tangent-Bitangent-Normal vertex shader
//
#pragma warning( disable : 3206 )

float4x4 object_to_world;
float4x4 world_to_object;
float4x4 world_to_view_clip;
float4x4 projection;

float4x4 world_to_light;
//float4x4 light_to_world;

float4x4 light_projection;

// View position in world space
float3 view_in_world;
float3 light_position_in_world;
float3 light_direction_in_world;

float time;
float3 distort_scale_coeff;
float3 distort_time_coeff;

//float4x4 createProj(float fovY, float aspect, float zn, float zf)
//{
//    return light_projection;
//    //float yScale = 1.0 / tan(fovY/2);
//    //float xScale = yScale / aspect;
////    float4x4 proj = {xScale, 0, 0, 0,
////                      0, yScale, 0, 0,
////                      0, 0, zf/(zf-zn), 1,
////                      0, 0, -zn*zf/(zf-zn), 0
////    };
////    return proj;
//}

float4x4 createView(float3 cameraPos, float3 target, float3 up)
{
	float3 zaxis = normalize(target - cameraPos);
	float3 xaxis = normalize(cross(up, zaxis));
	float3 yaxis = cross(zaxis, xaxis);
	
	float4x4 view = { xaxis.x, yaxis.x, zaxis.x, 0,
                      xaxis.y, yaxis.y, zaxis.y, 0,
                      xaxis.z, yaxis.z, zaxis.z, 0,
                      -dot(xaxis, cameraPos), -dot(yaxis, cameraPos), -dot(zaxis, cameraPos), 1
	};

	return view;
}

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
    float3 light_dir_in_tangent : TEXCOORD5;
    float4 light_view_position : TEXCOORD6;
    float3 normal_in_world : TEXCOORD7;
};

void main_vs(in input_vertex input, out output_vertex output)
{
    float4 position_in_world = mul(object_to_world, input.position);

    position_in_world.xyz += sin(time * distort_time_coeff + position_in_world.xyz) * distort_scale_coeff;
    
    output.position = mul(world_to_view_clip, position_in_world);
    
    //Transform normal to world space
    output.normal_in_world = mul(object_to_world, input.normal);
    
    vectors_in_object in_object = calculate_vectors_in_object(
        world_to_object, 
        input.position, 
        view_in_world,
        light_position_in_world,
        light_direction_in_world/*float3(1,1,1)*/);

    float3x3 object_to_tangent = calculate_object_to_tangent_orthonormal(
        input.tangent.xyz, input.normal, input.tangent.w);
        
    //output.tangent = object_to_tangent[0];
    //output.bitangent = object_to_tangent[1];
    //output.normal = object_to_tangent[2];
    
    output.texcoord = input.texcoord;
    output.view_in_tangent = mul(object_to_tangent, in_object.view_direction);
    output.light_in_tangent = mul(object_to_tangent, in_object.light_direction);
    output.light_dir_in_tangent = mul(object_to_tangent, in_object.directional_light_direction);// - output.light_in_tangent;
    
    // For attenuation, to avoid non-uniform scaling in light_in_tangent.
    output.light_in_world = position_in_world - light_position_in_world;
    
    //For occlusion
    output.screenPos = output.position;
    
    //For shadowmapping
    //float fovY = 90.0 / 360.0 * (2.0 * 3.1415f);
    //float aspect = 1.0; //1024 / 1024
    //float4x4 lightProj = transpose(createProj(fovY,aspect, 1.0, 200.0));
    //float4x4 lightView = transpose(createView(light_position_in_world,light_direction_in_world,float3(0,1,0)));
    //float4x4 lightWorldViewProj = mul(lightProj, lightView);
    //output.light_view_position = mul(lightWorldViewProj,position_in_world);
    
    //float4x4 lightWorldViewProj = mul(mul(object_to_world, world_to_light), lightProj);
    //output.light_view_position  = mul(input.position,lightWorldViewProj);
    
    //float4x4 lightWorldViewProj = mul(projection,world_to_light); //toimii k‰ytt‰‰ kameran projectiota
    float4x4 lightWorldViewProj = mul(light_projection, world_to_light);
    output.light_view_position  = mul(lightWorldViewProj, position_in_world);
}
