#include "light_variables2.h"

namespace pt { namespace d3d9 { namespace rendering {

light_variables2::light_variables2(
    light_shader_parameters const& parameters)
: m_shader(parameters.light_shader())
, m_shader_variables(parameters.shader_parameters())
{
}

light_variables2::~light_variables2()
{
}

std::shared_ptr<shaders::light_shader const> light_variables2::shader() const
{
    return m_shader;
}

parameter_collection const& light_variables2::parameters() const
{
    return m_shader_variables.parameters();
}

bool light_variables2::try_create_constants(constant_buffer const& constants)
{
    return m_shader_variables.try_create_constants(constants);
}

void light_variables2::update_target(float time,
    constant_buffer const& buffer) const
{
    m_shader_variables.update_target(time, buffer);
}

}}} // namespace pt::d3d9::rendering
