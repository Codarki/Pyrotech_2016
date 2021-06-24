#pragma once
#ifndef PT_D3D9_RENDERING_SURFACE_SHADER_PARAMETER_BUILDER_H
#define PT_D3D9_RENDERING_SURFACE_SHADER_PARAMETER_BUILDER_H

#include "shader_parameter_builder.h"
#include "pt/d3d9/graphics_device_fwd.h"

namespace pt { namespace d3d9 { namespace shaders {
    class surface_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering { namespace source {
    class shader_description;
}}}}

namespace pt { namespace d3d9 { namespace rendering {

class surface_shader_parameter_builder
{
public:
    surface_shader_parameter_builder(
        std::shared_ptr<shaders::surface_shader> shader,
        parameter_collection const& shared_parameters,
        graphics_device& device);

    surface_shader_parameters build(
        source::shader_description const& shader_description,
        parameter_collection const& parameter_overrides) const;

private: // data members
    std::shared_ptr<shaders::surface_shader> m_shader;
    shader_parameter_builder m_shader_parameter_builder;
};

}}} // namespace pt::d3d9::rendering

#endif
