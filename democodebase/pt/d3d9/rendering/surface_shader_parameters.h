#pragma once
#ifndef PT_D3D9_RENDERING_SURFACE_SHADER_PARAMETERS_H
#define PT_D3D9_RENDERING_SURFACE_SHADER_PARAMETERS_H

#include "shader_parameters.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {
    class surface_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering {

/// Defines parameters for a surface shader.
class surface_shader_parameters
{
public:
    surface_shader_parameters(std::shared_ptr<shaders::surface_shader> shader,
        shader_parameters shader_parameters);

    std::shared_ptr<shaders::surface_shader const> surface_shader() const;
    rendering::shader_parameters const& shader_parameters() const;

private: // data members
    std::shared_ptr<shaders::surface_shader> m_shader;
    rendering::shader_parameters m_shader_parameters;
};

}}} // namespace pt::d3d9::rendering

#endif
