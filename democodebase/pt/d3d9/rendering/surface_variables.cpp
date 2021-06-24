#include "surface_variables.h"

namespace pt { namespace d3d9 { namespace rendering {

surface_variables::surface_variables(
    surface_shader_parameters const& parameters)
: m_shader(parameters.surface_shader())
, m_shader_variables(parameters.shader_parameters())
{
}

surface_variables::~surface_variables()
{
}

std::shared_ptr<shaders::surface_shader const> surface_variables::shader() const
{
    return m_shader;
}

parameter_collection const& surface_variables::parameters() const
{
    return m_shader_variables.parameters();
}

bool surface_variables::try_create_constants(constant_buffer const& constants)
{
    return m_shader_variables.try_create_constants(constants);
}

void surface_variables::update_target(float time,
    constant_buffer const& buffer) const
{
    m_shader_variables.update_target(time, buffer);
}

}}} // namespace pt::d3d9::rendering
