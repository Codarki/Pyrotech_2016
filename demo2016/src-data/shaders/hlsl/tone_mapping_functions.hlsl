//static const float3 LUMINANCE = float3(0.2125f, 0.7154f, 0.0721f);
//static const float3x3 sRGB2XYZ = { 0.4124564,  0.3575761,  0.1804375, 0.2126729,  0.7151522,  0.0721750, 0.0193339,  0.1191920,  0.9503041 };
//static const float3x3 XYZ2sRGB = { 3.2404542, -1.5371385, -0.4985314, -0.9692660,  1.8760108,  0.0415560, 0.0556434, -0.2040259,  1.0572252 };
    
static const float3x3 RGB2XYZ = {
    0.5141364, 0.3238786,  0.16036376,
    0.265068,  0.67023428, 0.06409157,
    0.0241188, 0.1228178,  0.84442666};
    
static const float3x3 XYZ2RGB = {
    2.5651, -1.1665, -0.3986,
    -1.0217, 1.9777, 0.0439,
    0.0753, -0.2543, 1.1892 };
    
// RGB -> CIE XYZ conversion
float3 rgb_to_XYZ(float3 rgb_color)
{
    return mul(RGB2XYZ, rgb_color);
    //return mul(sRGB2XYZ, rgb_color);
}

// CIE XYZ -> CIE Yxy conversion
float3 XYZ_to_Yxy(float3 XYZ)
{
    float3 Yxy;
    Yxy.r = XYZ.g;
    // x = X / (X + Y + Z)
    Yxy.g = XYZ.r / (XYZ.r + XYZ.g + XYZ.b );
    // y = Y / (X + Y + Z) 
    Yxy.b = XYZ.g / (XYZ.r + XYZ.g + XYZ.b );
    
    //float temp = dot(float3(1.0, 1.0, 1.0), XYZ.rgb);
    //Yxy.gb = XYZ.rg / temp;
    return Yxy;
}

float3 rgb_to_Yxy(float3 rgb_color)
{
    return XYZ_to_Yxy(rgb_to_XYZ(rgb_color));
}

// CIE Yxy -> CIE XYZ conversion 
float3 Yxy_to_XYZ(float3 Yxy)
{
    float3 XYZ;
    // X = Y * x / y 
    XYZ.r = Yxy.r * Yxy.g / Yxy. b; 
    // copy luminance Y 
    XYZ.g = Yxy.r;
    // Z = Y * (1 - x - y) / y 
    XYZ.b = Yxy.r * (1 - Yxy.g - Yxy.b) / Yxy.b;
    return XYZ;
}

// CIE XYZ -> RGB conversion 
float3 XYZ_to_rgb(float3 XYZ)
{
    return mul(XYZ2RGB, XYZ);
    //return mul(XYZ2sRGB, XYZ);
}

float3 Yxy_to_rgb(float3 Yxy)
{
    return XYZ_to_rgb(Yxy_to_XYZ(Yxy));
}

float3 rgb_to_yuv(float3 col)
{
    float y = dot(col,float3(0.299,0.587,0.114));
    float u = (col.b - y) * 0.565;
    float v = (col.r - y) * 0.713;
    
    return float3(y,u,v);
}

// Krawczyk dynamic calculation of middle gray.
// Note: with constant average luminance, this can be constant.
float dynamic_middle_gray(float average_luminance)
{
    float denominator = 2 + log10(average_luminance + 1);
    return 1.03 - 2 / denominator;
}

// Reinhard
float3 Reinhard_ToneMap(float3 color_rgb, float adaptedLum, float middleGray, float white)
{
    float3 Yxy = rgb_to_Yxy(color_rgb);
    
    // Map average luminance to the middlegrey zone by scaling pixel luminance 
    // raise the value range a bit ... 
    const float LumScaled = Yxy.r * middleGray / (adaptedLum.x + 0.001f);
    
    const float whiteSquared = white * white;
    // Scale all luminance within a displayable range of 0 to 1 
    Yxy.r = (LumScaled * (1.0f + LumScaled / whiteSquared)) / (1.0f + LumScaled);
    
    return Yxy_to_rgb(Yxy);
}

float3 Reinhard_Brightpass(float3 color_rgb, float adaptedLum, float middleGray, float white, float threshold, float offset)
{
    float3 Yxy = rgb_to_Yxy(color_rgb);
    const float LumScaled = Yxy.r * middleGray / (adaptedLum.x + 0.001f);
    const float whiteSquared = white * white;
    const float LumThreshold = max((LumScaled * (1.0f + LumScaled / whiteSquared)) - threshold, 0.0f);
    const float LumBrightness = LumThreshold / (offset + LumThreshold);
    Yxy.r = LumBrightness;
    return Yxy_to_rgb(Yxy);
}
