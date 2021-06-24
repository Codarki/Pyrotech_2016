#include "parented_element.h"
#include <boost/assert.hpp>

namespace pt { namespace ui {

// parented_element

parented_element::parented_element(element* parent)
:   m_parent(parent)
{
    assert(m_parent);
}

void parented_element::invalidate()
{
    if (m_parent)
    {
        m_parent->invalidate();
        on_invalidate();
    }
}

void parented_element::on_invalidate()
{
}

}} // namespace pt::ui
