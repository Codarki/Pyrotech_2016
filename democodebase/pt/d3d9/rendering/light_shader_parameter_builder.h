#pragma once
#ifndef PT_D3D9_RENDERING_LIGHT_SHADER_PARAMETER_BUILDER_H
#define PT_D3D9_RENDERING_LIGHT_SHADER_PARAMETER_BUILDER_H

#include "shader_parameter_builder.h"
#include "light_shader_parameters.h"
#include "pt/d3d9/graphics_device_fwd.h"

namespace pt { namespace d3d9 { namespace shaders {
    class light_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering { namespace source {
    class shader_description;
}}}}

namespace pt { namespace d3d9 { namespace rendering {

class light_shader_parameter_builder
{
public:
    light_shader_parameter_builder(
        std::shared_ptr<shaders::light_shader> shader,
        parameter_collection const& shared_parameters,
        graphics_device& device);

    light_shader_parameters build(
        source::shader_description const& shader_description,
        parameter_collection const& parameter_overrides) const;

private: // data members
    std::shared_ptr<shaders::light_shader> m_shader;
    shader_parameter_builder m_shader_parameter_builder;
};

}}} // namespace pt::d3d9::rendering

#endif
