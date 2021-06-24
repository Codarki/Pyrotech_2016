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
float time_x;
float time_y;

float exposure;
float decay;
float density;
float weight;
float lightScrPosX;
float lightScrPosY;

texture2D child_texture;
sampler2D s0;

static const int NUM_SAMPLES = 200;

void main_ps(
    float2 texcoord : TEXCOORD0,
    out float4 color : COLOR0)
{
    color = tex2D(s0, texcoord);		

    float2 deltaTextCoord = float2( texcoord - float2(lightScrPosX, lightScrPosY));
    float2 uv = texcoord;
    deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
    float illuminationDecay = 1.0;
    
    for(int i=0; i < NUM_SAMPLES ; i++)
    {
            uv -= deltaTextCoord;
            float4 sample = tex2D(s0, uv );
            
            sample *= illuminationDecay * weight;
            
            color += sample;
            
            illuminationDecay *= decay;
    }		 
    
    color *= exposure;
    color = (color + tex2D(s0, texcoord ));
        
}