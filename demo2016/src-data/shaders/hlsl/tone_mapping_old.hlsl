// Vertex shader

struct input_vertex
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

void main_vs(inout input_vertex input)
{
};

// Pixel shader
        
texture2D source_texture;
texture2D add_texture;
sampler2D source_sampler;
sampler2D s1;
float middle_gray;
float adapted_luminance;

static const float3 luminance_vector  = float3(0.2125f, 0.7154f, 0.0721f);
static const float3 blue_shift_vector = float3(1.05f, 0.97f, 1.27f);

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    float3 original_color = tex2D(source_sampler, texcoord).xyz;

    float blue_shift_coefficient = saturate(1.0f - (adapted_luminance + 1.5) / 4.1);
    float3 rod_color = dot(original_color, luminance_vector) * blue_shift_vector;
    original_color = lerp(original_color, rod_color, blue_shift_coefficient);

    float scaled_luminance = middle_gray / (adapted_luminance + 0.001f);
    float compressed_luminance = scaled_luminance / (1.0f + scaled_luminance);
    color.xyz = saturate(original_color * compressed_luminance);

    color.xyz += tex2D(s1, texcoord).xyz;
    color.w = 1;
}
