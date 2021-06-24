#pragma once
#ifndef PT_D3D9_TEXTURE_2D_BUILDER_H
#define PT_D3D9_TEXTURE_2D_BUILDER_H

#include "native.h"
#include "graphics_device_fwd.h"
#include "texture_2d_fwd.h"
#include "pt/vector_fwd.h"
#include "pt/path_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {

class texture_2d_builder
{
public:
    struct parameters
    {
        parameters();

        UINT width;
        UINT height;
        UINT levels;
        DWORD usage;
        D3DFORMAT format;
        D3DPOOL pool;
    };

    texture_2d_builder(graphics_device& device);

    texture_2d_builder& width(UINT width);
    texture_2d_builder& height(UINT height);
    texture_2d_builder& resolution(UINT width, UINT height);
    texture_2d_builder& resolution(vector2i resolution);
    texture_2d_builder& levels(UINT levels);
    texture_2d_builder& usage(DWORD usage);
    texture_2d_builder& format(D3DFORMAT format);
    texture_2d_builder& pool(D3DPOOL pool);

    texture_2d build();
    texture_2d build(pt::path const& path);
    texture_2d build_immutable(pt::path const& path);

    std::shared_ptr<texture_2d> build_shared();
    std::shared_ptr<texture_2d> build_shared(pt::path const& path);
    std::shared_ptr<texture_2d> build_immutable_shared(pt::path const& path);

    std::unique_ptr<d3d9::texture_2d> build_unique_ptr();

private: // data members
    graphics_device* m_device;
    parameters m_parameters;
};

inline std::shared_ptr<texture_2d> load_texture_2d(graphics_device& device,
    pt::path const& path)
{
    return texture_2d_builder(device).build_shared(path);
}

inline std::shared_ptr<texture_2d> load_immutable_texture_2d(graphics_device& device,
    pt::path const& path)
{
    return texture_2d_builder(device).build_immutable_shared(path);
}

std::string bytes_in_formatted_text(size_t bytes);

}} // namespace pt::d3d9

#endif
