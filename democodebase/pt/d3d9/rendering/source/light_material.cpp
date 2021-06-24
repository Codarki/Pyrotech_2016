#include "light_material.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

// light_material

light_material::light_material(std::string const& name,
    light_shader_parameters const& shader_parameters)
: m_name(name)
, m_shader_parameters(shader_parameters)
{
}

std::string const& light_material::name() const
{
    return m_name;
}

light_shader_parameters const& light_material::shader_parameters() const
{
    return m_shader_parameters;
}

}}}} // namespace pt::d3d9::rendering::source
