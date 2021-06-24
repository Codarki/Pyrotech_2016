#include "setter_container.h"
#include "../variable_setter.h"
#include "pt/any_error.h"

namespace pt { namespace d3d9 { namespace rendering {  namespace detail {

setter_container::setter_container()
{
}

setter_container::~setter_container()
{
}

void setter_container::add(constant_buffer const* buffer,
    std::unique_ptr<variable_setter>&& setter)
{
    if (has(buffer, *setter))
        throw any_error("adding duplicate setter");

    m_setters[buffer].push_back(move(setter));
}

void setter_container::update_target(constant_buffer const* buffer,
    float time) const
{
    std::map<constant_buffer const*, setters>::const_iterator i
        = m_setters.find(buffer);
    if (i == m_setters.end())
    {
        // no setters for constants
        return;
    }

    setters const& setters = i->second;
    for (setters::const_iterator j = setters.begin(); j != setters.end(); ++j)
    {
        (*j)->update_target(time);
    }
}

bool setter_container::has_any_setter(constant_buffer const* buffer) const
{
    std::map<constant_buffer const*, setters>::const_iterator i
        = m_setters.find(buffer);
    if (i == m_setters.end())
        return false;

    return !i->second.empty();
}

bool setter_container::has(constant_buffer const* constants,
    variable_setter const& setter) const
{
    std::map<constant_buffer const*, setters>::const_iterator i
        = m_setters.find(constants);
    if (i == m_setters.end())
        return false;

    setters::const_iterator j = i->second.begin();
    setters::const_iterator j_end = i->second.end();

    for (; j != j_end; ++j)
    {
        variable_setter const& stored = **j;

        if (&setter == &stored)
            return true;
    }

    return false;
}

}}}} // namespace pt::d3d9::rendering::detail
