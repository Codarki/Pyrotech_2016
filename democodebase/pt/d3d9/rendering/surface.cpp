#include "surface.h"
#include "surface_variables.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

surface_variables_instance::surface_variables_instance(
    std::shared_ptr<surface_variables> variables,
    std::shared_ptr<surface_variables> pre_shader_variables)
: m_variables(variables)
, m_pre_shader_variables(pre_shader_variables)
{
    assert(m_variables);
}

variable_owner const& surface_variables_instance::variables() const
{
    return *m_variables;
}

variable_owner& surface_variables_instance::variables()
{
    return *m_variables;
}

variable_owner const& surface_variables_instance::pre_shader_variables() const
{
    assert(m_pre_shader_variables);
    return *m_pre_shader_variables;
}

variable_owner& surface_variables_instance::pre_shader_variables()
{
    assert(m_pre_shader_variables);
    return *m_pre_shader_variables;
}

std::shared_ptr<shaders::surface_shader const> surface_variables_instance::shader() const
{
    return m_variables->shader();
}

std::shared_ptr<shaders::surface_shader const> surface_variables_instance::pre_shader() const
{
    return m_pre_shader_variables->shader();
}

parameter_collection const& surface_variables_instance::parameters() const
{
    return m_variables->parameters();
}

//

surface::surface(std::vector<surface_variables_instance> instances)
: m_instances(instances)
{
}

std::vector<surface_variables_instance> const& surface::instances() const
{
    return m_instances;
}

std::vector<surface_variables_instance>& surface::instances()
{
    return m_instances;
}

}}} // namespace pt::d3d9::rendering
