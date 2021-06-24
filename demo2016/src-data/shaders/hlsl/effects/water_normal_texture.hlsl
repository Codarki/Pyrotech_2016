// Vertex shader

struct input_vertex
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct output_vertex
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

void main_vs(in input_vertex input, out output_vertex output)
{
	output = input;
}

// Pixel shader	

texture2D normal_texture;
sampler2D normal_sampler;

float normal_scale;
float angle_radians;
float3 speed_coeff; // default 1
float3 general_texcoord_offset; // default = 0
float3 layer1_texcoord_offset; // default 0
float3 layer2_texcoord_offset; // default 0

void main_ps(
	float2 texcoord : TEXCOORD0,
	out float4 color : COLOR0)
{
	float angle0 = angle_radians;
	float angle1 = -angle_radians;
	float2x2 rotation0 = {cos(angle0), -sin(angle0), sin(angle0), cos(angle0)};
	float2x2 rotation1 = {cos(angle1), -sin(angle1), sin(angle1), cos(angle1)};
	
	//float2 tmp_texcoord = texcoord
	//float2 direction = mul(rotation0, texcoord + general_texcoord_offset.xy);
	float2 texcoord_offset0 = general_texcoord_offset.xy * speed_coeff.xy;
	float2 texcoord_offset1 = general_texcoord_offset.xy * speed_coeff.xy + layer1_texcoord_offset.xy;
	float2 texcoord_offset2 = general_texcoord_offset.xy * speed_coeff.xy + layer2_texcoord_offset.xy;
	float2 direction0 = texcoord_offset0;
	float2 direction1 = mul(rotation0, texcoord_offset1);
	float2 direction2 = mul(rotation1, texcoord_offset2);
	
	//direction0 = direction1;
	//direction2 = direction1;
	
    float2 normal1_texcoord = texcoord + direction0;
    float2 normal2_texcoord = texcoord + direction1;
    float2 normal3_texcoord = texcoord + direction2;

    float3 N1 = tex2D(normal_sampler, normal1_texcoord).xyz * 2 - 1;
    float3 N2 = tex2D(normal_sampler, normal2_texcoord).xyz * 2 - 1;
    float3 N3 = tex2D(normal_sampler, normal3_texcoord).xyz * 2 - 1;
	
    N1.z *= normal_scale;
    N2.z *= normal_scale;
    N3.z *= normal_scale;
    N1 *= 0.5;
    N2 *= 0.3;
    N3 *= 0.2;
	
    float3 N = normalize(N1 + N2 + N3);

	color.xyz = N * 0.5 + 0.5;
	color.w = 1;
}
