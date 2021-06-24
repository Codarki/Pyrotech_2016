#include "child_frame.h"

namespace pt { namespace ui {

child_frame::~child_frame()
{
}

simple_bounded_frame::simple_bounded_frame()
{
}

simple_bounded_frame::simple_bounded_frame(box2i const& bounds,
    matrix3x3f const& local_to_parent)
:   m_bounds(bounds)
,   m_local_to_parent(local_to_parent)
{
}

void simple_bounded_frame::set_bounds(box2i const& bounds)
{
    m_bounds = bounds;
    set_bounds_called();
}

box2i const& simple_bounded_frame::bounds() const
{
    return m_bounds;
}

void simple_bounded_frame::set_local_to_parent_matrix(
    matrix3x3f const& matrix)
{
    m_local_to_parent = matrix;
}

matrix3x3f const& simple_bounded_frame::local_to_parent_matrix() const
{
    return m_local_to_parent;
}

void simple_bounded_frame::set_bounds_called()
{
}

// free functions

oriented_box2i bounds_in_parent(bounded_frame const& child)
{
    return transform(child.bounds(), child.local_to_parent_matrix());
}

}} // namespace pt::ui
