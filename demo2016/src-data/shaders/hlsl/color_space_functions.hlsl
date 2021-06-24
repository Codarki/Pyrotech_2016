float time_x;
float time_y;
float negative;


texture2D child_texture;
//texture2D source_texture;
sampler2D s0;

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    float4 texcol = tex2D(s0, texcoord);
    // 32 32 255
    // 255 - 32. 255- 32,255-255 = 223, 223, 0
    //float3 tonemapped_texcol = Reinhard_ToneMap(texcol.xyz, 0.17, 0.5, 1.0);
    //float3 neg_color = 1.0 - tonemapped_texcol;
    //float3 real_color = lerp(texcol.xyz, neg_color, negative);
    //color = float4(abs(neg_color) * length(texcol.xyz), texcol.w);
    
    //float3 Yxy = rgb_to_Yxy(texcol.xyz);

    // negation color for black pixel.
    //float max_white_point = 1;
    
    // max limit for negation is the luminance
    //float max_limit = negative * Yxy.r;
    
    float3 neg_color = abs(500 - texcol.xyz);
    
    color.xyz = texcol.xyz;
    color.w = texcol.w;
}