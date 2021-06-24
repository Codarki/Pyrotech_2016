#include "enum_utility.h"
#include "pt/any_error.h"
#include "pt/basic_types.h"
#include "pt/string_wrapper.h"

namespace pt { namespace d3d9 {

D3DDECLTYPE declaration_type(vertex_element_type type, size_t count)
{
    switch(type)
    {
    case element_float:
        switch(count)
        {
        case 1: return D3DDECLTYPE_FLOAT1;
        case 2: return D3DDECLTYPE_FLOAT2;
        case 3: return D3DDECLTYPE_FLOAT3;
        case 4: return D3DDECLTYPE_FLOAT4;
        }

    case element_uint8:
        if (count == 4)
            return D3DDECLTYPE_D3DCOLOR; // or D3DDECLTYPE_UBYTE4
        break;

    case element_int8:
        if (count == 4)
            return D3DDECLTYPE_UBYTE4N;
        break;

    case element_uint16:
        if (count == 2)
            return D3DDECLTYPE_USHORT2N;
        else if (count == 4)
            return D3DDECLTYPE_USHORT4N;
        break;

    case element_int16:
        if (count == 2)
            return D3DDECLTYPE_SHORT2;
        else if (count == 4)
            return D3DDECLTYPE_SHORT4;
        break;
    }

    // TODO D3DDECLTYPE_UDEC3, D3DDECLTYPE_DEC3N, D3DDECLTYPE_FLOAT16_2,
    // D3DDECLTYPE_FLOAT16_4

    throw any_error("no matching D3DDECLTYPE");
}

D3DDECLUSAGE d3d9_usage(std::string const& text)
{
    if (text == "POSITION")
        return D3DDECLUSAGE_POSITION;
    else if (text == "BLENDWEIGHT")
        return D3DDECLUSAGE_BLENDWEIGHT;
    else if (text == "BLENDINDICES")
        return D3DDECLUSAGE_BLENDINDICES;
    else if (text == "NORMAL")
        return D3DDECLUSAGE_NORMAL;
    else if (text == "PSIZE")
        return D3DDECLUSAGE_PSIZE;
    else if (text == "TEXCOORD")
        return D3DDECLUSAGE_TEXCOORD;
    else if (text == "TANGENT")
        return D3DDECLUSAGE_TANGENT;
    else if (text == "BINORMAL")
        return D3DDECLUSAGE_BINORMAL;
    else if (text == "POSITIONT")
        return D3DDECLUSAGE_POSITIONT;
    else if (text == "COLOR")
        return D3DDECLUSAGE_COLOR;
    else if (text == "FOG")
        return D3DDECLUSAGE_FOG;
    else if (text == "DEPTH")
        return D3DDECLUSAGE_DEPTH;
    else if (text == "SAMPLE")
        return D3DDECLUSAGE_SAMPLE;

    throw any_error("no matching D3DDECLUSAGE for " + text);
}

size_t declaration_size_in_bytes(D3DDECLTYPE type)
{
    switch(type)
    {
    case D3DDECLTYPE_UDEC3:     return 4; // doc says this is 30bit
    case D3DDECLTYPE_DEC3N:     return 4; // doc says this is 30bit
    }

    return declaration_element_count(type)
        * declaration_element_size_in_bytes(type);
}

size_t declaration_element_count(D3DDECLTYPE type)
{
    switch(type)
    {
    case D3DDECLTYPE_FLOAT1:    return 1;
    case D3DDECLTYPE_FLOAT2:    return 2;
    case D3DDECLTYPE_FLOAT3:    return 3;
    case D3DDECLTYPE_FLOAT4:    return 4;
    case D3DDECLTYPE_D3DCOLOR:  return 4;
    case D3DDECLTYPE_UBYTE4:    return 4;
    case D3DDECLTYPE_SHORT2:    return 2;
    case D3DDECLTYPE_SHORT4:    return 4;
    case D3DDECLTYPE_UBYTE4N:   return 4;
    case D3DDECLTYPE_SHORT2N:   return 2;
    case D3DDECLTYPE_SHORT4N:   return 4;
    case D3DDECLTYPE_USHORT2N:  return 2;
    case D3DDECLTYPE_USHORT4N:  return 4;
    case D3DDECLTYPE_UDEC3:     return 3;
    case D3DDECLTYPE_DEC3N:     return 3;
    case D3DDECLTYPE_FLOAT16_2: return 2;
    case D3DDECLTYPE_FLOAT16_4: return 4;
    }

    throw any_error("invalid D3DDECLTYPE");
}

size_t declaration_element_size_in_bytes(D3DDECLTYPE type)
{
    switch(type)
    {
    case D3DDECLTYPE_FLOAT1:    return sizeof(float);
    case D3DDECLTYPE_FLOAT2:    return sizeof(float);
    case D3DDECLTYPE_FLOAT3:    return sizeof(float);
    case D3DDECLTYPE_FLOAT4:    return sizeof(float);
    case D3DDECLTYPE_D3DCOLOR:  return sizeof(uint8);
    case D3DDECLTYPE_UBYTE4:    return sizeof(uint8);
    case D3DDECLTYPE_SHORT2:    return sizeof(int16);
    case D3DDECLTYPE_SHORT4:    return sizeof(int16);
    case D3DDECLTYPE_UBYTE4N:   return sizeof(int8);
    case D3DDECLTYPE_SHORT2N:   return sizeof(int16);
    case D3DDECLTYPE_SHORT4N:   return sizeof(int16);
    case D3DDECLTYPE_USHORT2N:  return sizeof(uint16);
    case D3DDECLTYPE_USHORT4N:  return sizeof(uint16);
    case D3DDECLTYPE_FLOAT16_2: return 2;
    case D3DDECLTYPE_FLOAT16_4: return 4;
    }

    // D3DDECLTYPE_UDEC3 and D3DDECLTYPE_DEC3N are 10bits

    throw any_error("invalid D3DDECLTYPE");
}

vertex_element_type element_type(D3DDECLTYPE type)
{
    switch(type)
    {
    case D3DDECLTYPE_FLOAT1:
    case D3DDECLTYPE_FLOAT2:
    case D3DDECLTYPE_FLOAT3:
    case D3DDECLTYPE_FLOAT4:    return element_float;
    case D3DDECLTYPE_D3DCOLOR:
    case D3DDECLTYPE_UBYTE4:    return element_uint8;
    case D3DDECLTYPE_UBYTE4N:   return element_int8;
    case D3DDECLTYPE_SHORT2:
    case D3DDECLTYPE_SHORT4:
    case D3DDECLTYPE_SHORT2N:
    case D3DDECLTYPE_SHORT4N:   return element_int16;
    case D3DDECLTYPE_USHORT2N:
    case D3DDECLTYPE_USHORT4N:  return element_uint16;
    case D3DDECLTYPE_FLOAT16_2:
    case D3DDECLTYPE_FLOAT16_4: return element_float_16;
    }

    // D3DDECLTYPE_UDEC3 and D3DDECLTYPE_DEC3N are 10bits

    throw any_error("invalid D3DDECLTYPE");
}

size_t format_size_in_bits(D3DFORMAT format)
{
    switch(format)
    {
    case D3DFMT_R8G8B8: return 24;
    case D3DFMT_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
    case D3DFMT_A8B8G8R8:
    case D3DFMT_X8B8G8R8:
    case D3DFMT_G16R16:
    case D3DFMT_A2R10G10B10:
    case D3DFMT_A2B10G10R10:
    case D3DFMT_X8L8V8U8:
    case D3DFMT_Q8W8V8U8:
    case D3DFMT_V16U16:
    case D3DFMT_A2W10V10U10: return 32;
    case D3DFMT_R5G6B5:
    case D3DFMT_X1R5G5B5:
    case D3DFMT_A1R5G5B5:
    case D3DFMT_A8R3G3B2:
    case D3DFMT_X4R4G4B4:
    case D3DFMT_A4R4G4B4:
    case D3DFMT_A8L8:
    case D3DFMT_A8P8:
    case D3DFMT_V8U8:
    case D3DFMT_L6V5U5: return 16;
    case D3DFMT_R3G3B2:
    case D3DFMT_P8:
    case D3DFMT_L8:
    case D3DFMT_A8:
    case D3DFMT_A4L4: return 8;
    case D3DFMT_A16B16G16R16: return 64;
    case D3DFMT_UYVY: throw any_error("D3DFMT_UYVY bitsize unknown");
    case D3DFMT_R8G8_B8G8: throw any_error("D3DFMT_R8G8_B8G8 bitsize unknown");
    case D3DFMT_YUY2: throw any_error("D3DFMT_YUY2 bitsize unknown");
    case D3DFMT_G8R8_G8B8: throw any_error("D3DFMT_G8R8_G8B8 bitsize unknown");
    case D3DFMT_DXT1: return 4;
    case D3DFMT_DXT2:
    case D3DFMT_DXT3:
    case D3DFMT_DXT4:
    case D3DFMT_DXT5: return 8;

    case D3DFMT_D15S1:
    case D3DFMT_D16:
    case D3DFMT_D16_LOCKABLE: return 16;
    case D3DFMT_D24S8:
    case D3DFMT_D24X8:
    case D3DFMT_D24X4S4:
    case D3DFMT_D32F_LOCKABLE:
    case D3DFMT_D24FS8:
    case D3DFMT_D32: return 32;

    case D3DFMT_L16: return 16;
    case D3DFMT_VERTEXDATA: throw any_error("D3DFMT_VERTEXDATA bitsize unknown");
    case D3DFMT_INDEX16: return 16;
    case D3DFMT_INDEX32: return 32;
    case D3DFMT_Q16W16V16U16: return 64;
    case D3DFMT_MULTI2_ARGB8: throw any_error("D3DFMT_MULTI2_ARGB8 bitsize unknown");

    // s10e5 formats (16-bits per channel)
    case D3DFMT_R16F: return 16;
    case D3DFMT_G16R16F: return 32;
    case D3DFMT_A16B16G16R16F: return 64;

    // IEEE s23e8 formats (32-bits per channel)
    case D3DFMT_R32F: return 32;
    case D3DFMT_G32R32F: return 64;
    case D3DFMT_A32B32G32R32F: return 128;

    case D3DFMT_CxV8U8: return 16;
    }

    throw any_error("invalid D3DFORMAT");
}

}} // namespace pt::d3d9
