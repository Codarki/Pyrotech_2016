#include "bounded_event.h"
#include "pt/std/algorithm.h"

namespace pt { namespace ui {

bounded_event::bounded_event(box2i const& bounds)
{
    m_bounds.push(bounds);
}

box2i const& bounded_event::bounds() const
{
    return m_bounds.top();
}

void bounded_event::push(box2i const& child_bounds)
{
    vector2i min_corner(min_position(bounds()));
    vector2i max_corner(max_position(bounds()));

    min_corner[0] = std::max(min_corner[0], min_position(child_bounds)[0]);
    min_corner[1] = std::max(min_corner[1], min_position(child_bounds)[1]);

    max_corner[0] = std::min(max_corner[0], max_position(child_bounds)[0]);
    max_corner[1] = std::min(max_corner[1], max_position(child_bounds)[1]);

    m_bounds.push(box_from_min_max(min_corner, max_corner));
}

void bounded_event::pop()
{
    m_bounds.pop();
}

}} // namespace pt::ui
