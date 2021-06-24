#pragma once
#ifndef PT_D3D9_TEXTURE_2D_UTILS_H
#define PT_D3D9_TEXTURE_2D_UTILS_H

#include "pt/path.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/d3d9/texture_2d_builder.h"

namespace pt { namespace d3d9 {

class graphics_device;

inline std::shared_ptr<d3d9::texture_2d> find_or_load(
    pt::path const& path, graphics_device& device)
{
    std::shared_ptr<d3d9::texture_2d> texture
        = d3d9::default_texture_2d_collection().find(path.string());

    if (!texture)
    {
        texture = d3d9::load_immutable_texture_2d(device, path);
        d3d9::default_texture_2d_collection().add(path.string(), texture);
    }

    return texture;
}

}} // namespace pt::d3d9

#endif
