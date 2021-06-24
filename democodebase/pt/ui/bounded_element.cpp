#include "bounded_element.h"

namespace pt { namespace ui {

bounded_element::bounded_element()
{
}

bounded_element::bounded_element(box2i const& bounds)
:   simple_bounded_frame(bounds, matrix3x3f())
{
}

//box2i bounded_element::local_bounds() const
//{
//    return box2i(m_bounds.center() - min_position(m_bounds), m_bounds.size());
//}

void bounded_element::set_bounds_called()
{
    invalidate();
    on_set_bounds();
}

void bounded_element::on_set_bounds()
{
}

}} // namespace pt::ui
