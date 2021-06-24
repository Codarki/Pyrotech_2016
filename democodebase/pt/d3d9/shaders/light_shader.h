#pragma once
#ifndef PT_D3D9_SHADERS_LIGHT_SHADER_H
#define PT_D3D9_SHADERS_LIGHT_SHADER_H

#include "light_shader_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 { namespace shaders {

class light_shader
{
public:
    virtual ~light_shader();

public: // interface which subclasses must implement
    virtual std::string const& name() const = 0;
    virtual std::string const& pixel_shader() const = 0;
    virtual config_node const& parameters() const = 0;
    virtual config_node const& sampler_states() const = 0;
};

}}} // namespace pt::d3d9::shaders

#endif
