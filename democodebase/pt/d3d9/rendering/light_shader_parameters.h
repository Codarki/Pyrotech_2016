#pragma once
#ifndef PT_D3D9_RENDERING_LIGHT_SHADER_PARAMETERS_H
#define PT_D3D9_RENDERING_LIGHT_SHADER_PARAMETERS_H

#include "shader_parameters.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {
    class light_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering {

class light_shader_parameters
{
public:
    light_shader_parameters(std::shared_ptr<shaders::light_shader> shader,
        shader_parameters shader_parameters);

    std::shared_ptr<shaders::light_shader const> light_shader() const;
    rendering::shader_parameters const& shader_parameters() const;

private: // data members
    std::shared_ptr<shaders::light_shader> m_shader;
    rendering::shader_parameters m_shader_parameters;
};

}}} // namespace pt::d3d9::rendering

#endif
