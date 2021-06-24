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
float focus;
float range;
texture2D source_texture;
sampler2D s0;
texture2D blurred_texture;
sampler2D s1;
texture2D depth_texture;
sampler2D s2;
float texel_width;
float texel_height;
static const float2 texel = float2(texel_width,texel_height);
float blur_rgb_dist;
float gain;
float threshold;

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    float4 sharp    = tex2D(s0, texcoord);
    
    //Distort blurred images rgb values blur_rgb_dist is external parameter
    float4 blur;
    blur.r = tex2D(s1,texcoord+float2(0.0, 1.0)*texel*blur_rgb_dist).r;
    blur.g = tex2D(s1,texcoord+float2(-0.866, -0.5)*texel*blur_rgb_dist).g;
    blur.b = tex2D(s1,texcoord+float2(0.866, -0.5)*texel*blur_rgb_dist).b;
    blur.a = 1;
    
    //Calculate luminance value and boost those in blurred image
    float3 lumcoeff = float3(0.299, 0.587, 0.114);
    float lum = dot(blur.rgb, lumcoeff);
    float th = max((lum - threshold)*gain, 0.0);
    blur.rgb = blur.rgb + lerp(float3(0.0,0.0,0.0), blur.rgb, th*saturate(range * abs(focus - tex2D(s2, texcoord).a)));
    
    //interpolate between sharp and blur texture using depth value, focus and range
    color = lerp(sharp, blur, saturate(range * abs(focus - tex2D(s2, texcoord).a)));    
}
