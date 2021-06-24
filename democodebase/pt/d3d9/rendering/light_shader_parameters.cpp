#include "light_shader_parameters.h"

namespace pt { namespace d3d9 { namespace rendering {

// light_shader_parameters

light_shader_parameters::light_shader_parameters(
    std::shared_ptr<shaders::light_shader> shader,
    rendering::shader_parameters shader_parameters)
: m_shader(shader)
, m_shader_parameters(shader_parameters)
{
}

std::shared_ptr<shaders::light_shader const>
    light_shader_parameters::light_shader() const
{
    return m_shader;
}

rendering::shader_parameters const&
    light_shader_parameters::shader_parameters() const
{
    return m_shader_parameters;
}

}}} // namespace pt::d3d9::rendering
