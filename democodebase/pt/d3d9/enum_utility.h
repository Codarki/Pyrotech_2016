#ifndef PT_D3D9_ENUM_UTILITY_H
#define PT_D3D9_ENUM_UTILITY_H

#include "native.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {

enum vertex_element_type
{
    element_int8,
    element_uint8,
    element_int16,
    element_uint16,
    element_float,
    element_float_16,
};

D3DDECLTYPE declaration_type(vertex_element_type type, size_t count);
D3DDECLUSAGE d3d9_usage(std::string const& text);

size_t declaration_size_in_bytes(D3DDECLTYPE type);
size_t declaration_element_count(D3DDECLTYPE type);
size_t declaration_element_size_in_bytes(D3DDECLTYPE type);
vertex_element_type element_type(D3DDECLTYPE type);

size_t format_size_in_bits(D3DFORMAT format);

}} // namespace pt::d3d9

#endif
