#include "light_properties.h"
#include "light_variables2.h"

namespace pt { namespace d3d9 { namespace rendering {

light_properties::light_properties(
    std::shared_ptr<light_variables2> variables)
: m_variables(variables)
{
}

variable_owner const& light_properties::variables() const
{
    return *m_variables;
}

variable_owner& light_properties::variables()
{
    return *m_variables;
}

std::shared_ptr<shaders::light_shader const> light_properties::shader() const
{
    return m_variables->shader();
}

parameter_collection const& light_properties::parameters() const
{
    return m_variables->parameters();
}

}}} // namespace pt::d3d9::rendering
