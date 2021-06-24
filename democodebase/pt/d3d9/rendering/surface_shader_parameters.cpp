#include "surface_shader_parameters.h"

namespace pt { namespace d3d9 { namespace rendering {

// surface_shader_parameters

surface_shader_parameters::surface_shader_parameters(
    std::shared_ptr<shaders::surface_shader> shader,
    rendering::shader_parameters shader_parameters)
: m_shader(shader)
, m_shader_parameters(shader_parameters)
{
}

std::shared_ptr<shaders::surface_shader const>
    surface_shader_parameters::surface_shader() const
{
    return m_shader;
}

rendering::shader_parameters const&
    surface_shader_parameters::shader_parameters() const
{
    return m_shader_parameters;
}

}}} // namespace pt::d3d9::rendering
