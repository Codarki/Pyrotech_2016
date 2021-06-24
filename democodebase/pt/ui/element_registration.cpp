#include "element_registration.h"
#include "element_registry.h"

namespace pt { namespace ui {

element_registration::element_registration(
	boost::shared_ptr<element_type> const& type)
:   m_type(type)
{
    element_registry::instance().add(m_type);
}

element_registration::~element_registration()
{
    element_registry::instance().remove(m_type);
}

boost::shared_ptr<element_type const> element_registration::type() const
{
    return m_type;
}

}} // namespace pt::ui
