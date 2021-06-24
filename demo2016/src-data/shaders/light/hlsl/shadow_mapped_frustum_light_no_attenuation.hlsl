
static float4 g_light_position_in_view;

texture2D frustum_light_mask_texture;
texture2D shadow_texture;

sampler2D frustum_light_mask_sampler;
sampler2D shadow_sampler;
float shadow_bias;

float2 shadow_offsets[4] = {float2(0,0), float2(1,0), float2(1,1), float2(0,1)};

float2 poisson_disk[4] = {
  float2( -0.94201624, -0.39906216 ),
  float2( 0.94558609, -0.76890725 ),
  float2( -0.094184101, -0.92938870 ),
  float2( 0.34495938, 0.29387760 )
};

float3 do_the_light()
{
}

void initialize_light(float4 light_position_in_view)
{
    g_light_position_in_view = light_position_in_view;
}

float attenuation(float3 unnormalized_light_direction)
{
	return 1;
}

float3 calculate_light(float3 unnormalized_light_direction, float3 light_color)
{
    float3 I = light_color;
    
    float2 light_texcoord = float2(0, 0);
    //Calculate projected light view position
    light_texcoord.x = g_light_position_in_view.x / g_light_position_in_view.w * 0.5 + 0.5;
    light_texcoord.y = -g_light_position_in_view.y / g_light_position_in_view.w * 0.5 + 0.5;

    if (g_light_position_in_view.w > 0 && light_texcoord.x >= 0 && light_texcoord.x <= 1 && light_texcoord.y >= 0 && light_texcoord.y <= 1)
    {
        //float shadow_bias = 1.00111;//0.00011; //0.0051 good when in engine lightcam planes(1.0,200)  and in shader planes (1.0,100)
                                     //0.00011 good when in engine lightcam planes(1.0 200) and in shader planes(1.0 200)
        
		float3 total_mask = 0;
        for (int i = 0; i < 4; ++i)
		{
			float3 sample_I = 0;
			//float2 texcoord2 = light_texcoord + shadow_offsets[i] * 0.001;
			float2 texcoord2 = light_texcoord + poisson_disk[i] * 0.001;
			float shadow_depth = tex2D(shadow_sampler, texcoord2).r;
			float current_depth = 1.0 - (g_light_position_in_view.z / g_light_position_in_view.w) + shadow_bias;
			
			if (current_depth < shadow_depth)
			{
				//sample_I = 0;
			}
			else
			{
				float3 mask = tex2D(frustum_light_mask_sampler, texcoord2).rgb;
				total_mask += mask;
			}
		}
		I *= total_mask / 4;

    }
    else
    {
        // We are not in light frustum
        I = 0;
    }
        
    return I;
}
