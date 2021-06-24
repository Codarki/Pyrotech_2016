#ifndef PT_D3D9_TEXTURE_2D_VALUE_UTILS_H
#define PT_D3D9_TEXTURE_2D_VALUE_UTILS_H

#include "pt/d3d9/texture_2d_value.h"
#include "constant_texture_2d_source.h"
#include "texture_2d_fwd.h"

namespace pt { namespace d3d9 {

class texture_2d_value;
class texture_2d;

inline std::shared_ptr<texture_2d_value> create_constant_texture_2d_value(
    std::shared_ptr<d3d9::texture_2d> texture)
{
    std::shared_ptr<d3d9::constant_texture_2d_source> source(
        new d3d9::constant_texture_2d_source(texture));

    return std::shared_ptr<d3d9::texture_2d_value>(
            new d3d9::texture_2d_value(std::move(source)));
}

}} // namespace pt::d3d9

#endif
