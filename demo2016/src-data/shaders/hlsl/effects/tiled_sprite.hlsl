texture2D low_resolution_texture1;
texture2D low_resolution_texture2;
texture2D low_resolution_texture3;
texture2D sprite_texture;

sampler2D low_resolution_sampler1;
sampler2D low_resolution_sampler2;
sampler2D low_resolution_sampler3;
sampler2D sprite_sampler;

void main_ps(
    float2 texcoord : TEXCOORD0,
    float3 view_in_tangent : TEXCOORD1,
    float3 light_in_tangent : TEXCOORD2,
    float3 light_in_world : TEXCOORD3,
    float4 screenPos : TEXCOORD4,
    out float4 color : COLOR0)
{
	texcoord = (screenPos.xy/screenPos.w);
	texcoord.y = -texcoord.y;
	texcoord /= 2.0;
	texcoord += 0.5;

    float3 tex1 = tex2D(low_resolution_sampler1, texcoord).xyz;
    float3 tex2 = tex2D(low_resolution_sampler2, texcoord).xyz;
    float3 tex3 = tex2D(low_resolution_sampler3, texcoord).xyz;
		
	// See shared_textures.xml
	float2 texcoord1 = texcoord * 16; // 30
	float2 texcoord2 = texcoord * 32; // 44
	float2 texcoord3 = texcoord * 64; // 81

    float3 sprite_tex1 = tex2D(sprite_sampler, texcoord1).xyz;
    float3 sprite_tex2 = tex2D(sprite_sampler, texcoord2).xyz;
    float3 sprite_tex3 = tex2D(sprite_sampler, texcoord3).xyz;
	
	// convert from gamma 2.2 to gamma 1.0
	sprite_tex1 = pow(abs(sprite_tex1), 2.2);
	sprite_tex2 = pow(abs(sprite_tex2), 2.2);
	sprite_tex3 = pow(abs(sprite_tex3), 2.2);
	
	color.xyz = 0;
    color.xyz += tex1 * sprite_tex1 * 0.2;
    color.xyz += tex2 * sprite_tex2 * 0.3;
    color.xyz += tex3 * sprite_tex3 * 0.5;
	//color.xyz *= 0.33;
    color.w = 1;
}
