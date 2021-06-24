// Vertex shader

struct input_vertex
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

void main_vs(inout input_vertex input)
{
}

// Pixel shader

texture2D source_texture;
sampler2D source_sampler;
float time;
float2 speed;
float4 cutoff_min; // = float4(0.2, 0.2, 0.2, 0);
float4 cutoff_max; // = float4(0.5, 0.5, 0.5, 1);

float4 sample_source(float2 texcoord, float mip_level)
{
    //return tex2D(source_sampler, texcoord);
    
    float4 texx = float4(texcoord.xy, 0, mip_level);
	
	// Use green channel as alpha.
    return tex2Dlod(source_sampler, texx).xyzy;
}

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    float2 direction0 = normalize(float2( 1, 0));
    float2 direction1 = normalize(float2(-1, 0));
    float2 direction2 = normalize(float2(0,  1));
    float2 direction3 = normalize(float2(0, -1));

    float2 direction4 = normalize(float2( 1,  1));
    float2 direction5 = normalize(float2(-1,  1));
    float2 direction6 = normalize(float2(-1, -1));
    float2 direction7 = normalize(float2( 1, -1));
    
    //float horizontal_speed = 0.05;
    //float vertical_speed = 0.05;
    //float2 speed = float2(horizontal_speed, vertical_speed);
    float2 speed0 = speed * 1.0f;
    float2 speed1 = speed * 1.0f;
    float2 speed2 = speed * 1.0f;
    float2 speed3 = speed * 1.0f;
    float2 speed4 = speed * 1.0f;
    float2 speed5 = speed * 1.0f;
    float2 speed6 = speed * 1.0f;
    float2 speed7 = speed * 1.0f;
    
    direction0 *= speed0;
    direction1 *= speed1;
    direction2 *= speed2;
    direction3 *= speed3;
    direction4 *= speed4;
    direction5 *= speed5;
    direction6 *= speed6;
    direction7 *= speed7;
            
    float2 position_offset0 = float2(0.0, 0.0);
    float2 position_offset1 = float2(0.52, 0.4);
    float2 position_offset2 = float2(0.29, 0.8);
    float2 position_offset3 = float2(0.79, 0.45);
    float2 position_offset4 = float2(0.8, 0.2);
    float2 position_offset5 = float2(0.32, 0.8);
    float2 position_offset6 = float2(0.19, 0.49);
    float2 position_offset7 = float2(0.69, 0.71);
    
    float2 offset0 = direction0 * time + position_offset0;
    float2 offset1 = direction1 * time + position_offset1;
    float2 offset2 = direction2 * time + position_offset2;
    float2 offset3 = direction3 * time + position_offset3;
    float2 offset4 = direction4 * time + position_offset4;
    float2 offset5 = direction5 * time + position_offset5;
    float2 offset6 = direction6 * time + position_offset6;
    float2 offset7 = direction7 * time + position_offset7;

    float2 texcoord0 = texcoord + offset0;
    float2 texcoord1 = texcoord + offset1;
    float2 texcoord2 = texcoord + offset2;
    float2 texcoord3 = texcoord + offset3;
    float2 texcoord4 = texcoord + offset4;
    float2 texcoord5 = texcoord + offset5;
    float2 texcoord6 = texcoord + offset6;
    float2 texcoord7 = texcoord + offset7;
    
    float4 C0 = sample_source(texcoord0, 0);
    float4 C1 = sample_source(texcoord1, 0);
    float4 C2 = sample_source(texcoord2, 0);
    float4 C3 = sample_source(texcoord3, 0);
    float4 C4 = sample_source(texcoord4, 0);
    float4 C5 = sample_source(texcoord5, 0);
    float4 C6 = sample_source(texcoord6, 0);
    float4 C7 = sample_source(texcoord7, 0);

    // Fire 1
    //float4 cutoff_min = float4(0.0, 0.1, 0.2, 0);
    //float4 cutoff_max = float4(0.3, 0.4, 0.5, 1);

    // Fire 2
    //float4 cutoff_min = float4(0.2, 0.3, 0.4, 0);
    //float4 cutoff_max = float4(0.4, 0.5, 0.7, 1);

    // Fire 3
    //float4 cutoff_min = float4(0.3, 0.4, 0.5, 0);
    //float4 cutoff_max = float4(0.4, 0.5, 0.6, 1);

    // Fire 3
    //float4 cutoff_min = float4(0.55, 0.4, 0.1, 0);
    //float4 cutoff_max = float4(0.6, 0.45, 0.15, 1);
    
    // Fire 4
    //float4 cutoff_min = float4(0.2, 0.3, 0.4, 0);
    //float4 cutoff_max = float4(0.4, 0.5, 0.6, 1);
    
    // float4 cutoff_min = float4(0.2, 0.2, 0.2, 0);
    // float4 cutoff_max = float4(0.5, 0.5, 0.5, 1);

//    color = (C0 * C1 * C2 * C3);
//    color = smoothstep(0.0, 0.1, color);

    //color = (C0 + C1 + C2 + C3) / 4.0;
    color = (C0 + C1 + C2 + C3 + C4 + C5 + C6 + C7) / 8.0;
    //color = (C0 + C1 + C2 + C3 + C4 + C5 + C6 + C7);
    //color = (C0 + C2) / 2;
    color = smoothstep(cutoff_min, cutoff_max, color);

    //float4 inv_color = 1.0 - color;
    //color = 1.0 - pow(inv_color, 8);

////    float max_value = 0.5;
////    color = saturate(color + (1.0 - max_value));

//    color = pow(color, 1);

//    color = clamp(color, 0.5, 1.0);

//    float4 original_color = tex2D(source_sampler, texcoord);
//    color = original_color;
//    color.x = time % 1.0;
//    color.w *= alpha;

//------

/*
    float3 original_color = color.xyz;
    
    float adapted_luminance = 0.2;
    float middle_gray = 0.1;

    float scaled_luminance = middle_gray / (adapted_luminance + 0.001f);
    float compressed_luminance = scaled_luminance / (1.0f + scaled_luminance);
    color.xyz = saturate(original_color * compressed_luminance);
*/

	//color.w = saturate(color.w);
    //color.w = 0.5;
}

