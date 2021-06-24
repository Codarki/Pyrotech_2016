#include "semantic.h"
#include "pt/boost/lexical_cast.h"

namespace pt { namespace d3d9 {

semantic::semantic(std::string const& name)
:   m_name(name)
,   m_index(0)
{
}

semantic::semantic(std::string const& name, size_t index)
:   m_name(name)
,   m_index(index)
{
}

std::string semantic::text() const
{
    return m_name + lexical_cast<std::string>(m_index);
}

std::string const& semantic::name() const
{
    return m_name;
}

size_t semantic::index() const
{
    return m_index;
}

bool semantic::operator==(semantic const& x) const
{
    return m_name == x.m_name && m_index == x.m_index;
}

bool semantic::operator<(semantic const& x) const
{
    if (m_name < x.m_name)
        return false;
    if (x.m_name < m_name)
        return true;

    return m_index < x.m_index;
}

}} // namespace pt::d3d9
