#include "light_shader_parameter_builder.h"
#include "pt/d3d9/shaders/light_shader.h"

namespace pt { namespace d3d9 { namespace rendering {

// light_shader_parameter_builder

light_shader_parameter_builder::light_shader_parameter_builder(
    std::shared_ptr<shaders::light_shader> shader,
    parameter_collection const& shared_parameters,
    graphics_device& device)
: m_shader(shader)
, m_shader_parameter_builder(m_shader->parameters(), m_shader->sampler_states(), shared_parameters, device)
{
}

light_shader_parameters light_shader_parameter_builder::build(
    source::shader_description const& shader_description,
    parameter_collection const& parameter_overrides) const
{
    shader_parameters shader_parameters = m_shader_parameter_builder.build(shader_description, parameter_overrides);
    return light_shader_parameters(m_shader, shader_parameters);
}

}}} // namespace pt::d3d9::rendering
