#include "simple_variable_owner.h"
#include "constant_setter.h"
#include "pt/d3d9/constant_buffer.h"

namespace pt { namespace d3d9 { namespace rendering {

simple_variable_owner::simple_variable_owner()
{
}

simple_variable_owner::~simple_variable_owner()
{
}

bool simple_variable_owner::try_create_constants(
    constant_buffer const& constants)
{
    // TODO Return value has no logic and it's not even checked.

    if (constants.count() == 0)
        return false;

    // Hacky way of detecting if this composite has already created setters
    // for given constant buffer.
    if (m_setters.has_any_setter(&constants))
        return false;

    for (size_t i = 0; i < constants.count(); ++i)
    {
        d3d9::constant const& c = constants.at(i);

        if (std::unique_ptr<constant_setter> setter = on_create_constant(c))
        {
            setter->associate_to(constants, c);

            m_setters.add(&constants, move(setter));
        }
        else if (std::unique_ptr<variable_setter> setter2
            = on_create_setter(constants, i))
        {
            m_setters.add(&constants, move(setter2));
        }
    }

    return false;
}

void simple_variable_owner::update_target(float time,
    constant_buffer const& buffer) const
{
    m_setters.update_target(&buffer, time);
}

std::unique_ptr<variable_setter> simple_variable_owner::on_create_setter(
    constant_buffer const&, size_t) const
{
    return std::unique_ptr<variable_setter>();
}

}}} // namespace pt::d3d9::rendering
