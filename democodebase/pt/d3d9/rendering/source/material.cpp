#include "material.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

material_shader::material_shader(
    surface_shader_parameters const& shader_parameters,
    surface_shader_parameters const& pre_shader_parameters)
: m_shader_parameters(shader_parameters)
, m_pre_shader_parameters(pre_shader_parameters)
{
}

surface_shader_parameters const& material_shader::shader_parameters() const
{
    return m_shader_parameters;
}

surface_shader_parameters const& material_shader::pre_shader_parameters() const
{
    return m_pre_shader_parameters;
}

//

material::material(std::string const& name,
    std::vector<material_shader> shaders)
: m_name(name)
, m_shaders(shaders)
{
}

std::string const& material::name() const
{
    return m_name;
}

std::vector<material_shader> const& material::shaders() const
{
    return m_shaders;
}

}}}} // namespace pt::d3d9::rendering::source
