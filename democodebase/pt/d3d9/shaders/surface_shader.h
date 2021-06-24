#pragma once
#ifndef PT_D3D9_SHADERS_SURFACE_SHADER_H
#define PT_D3D9_SHADERS_SURFACE_SHADER_H

#include "surface_shader_fwd.h"
#include "pt/path.h"
#include "pt/config_node_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {

class stream_collection;

/// Provides HLSL source code for shaders.
class surface_shader
{
public:
    virtual ~surface_shader();

public: // interface which subclasses must implement
    virtual std::string const& pixel_shader() const = 0;
    virtual std::string const& pixel_shader_name() const = 0;
    virtual std::string const& vertex_shader() const = 0;
    virtual std::string const& vertex_shader_name() const = 0;
    virtual std::string const& name() const = 0;
    virtual stream_collection const& streams() const = 0;
    virtual config_node const& parameters() const = 0;
    virtual config_node const& sampler_states() const = 0;
    virtual bool is_additive_blended() const = 0;
    virtual bool is_transparent() const = 0;
    virtual bool is_alpha_tested() const = 0;
    virtual bool is_pixel_required_for_depth() const = 0;
    virtual int d3d_cullmode() const = 0;
};

std::shared_ptr<surface_shader> load_surface_shader(pt::path const& path);

}}} // namespace pt::d3d9::shaders

#endif
