#include "corner.h"
#include "pt/boost/assert.h"

namespace p3d_format {

// corner

corner::corner(p3d_format::point* point)
:   m_point(point)
,   m_polygon(0)
{
}

p3d_format::point const* corner::point() const
{
    return m_point;
}

p3d_format::polygon const* corner::polygon() const
{
    return m_polygon;
}

void corner::set_polygon(p3d_format::polygon* polygon)
{
    assert(!m_polygon);
    m_polygon = polygon;
}

} // namespace p3d_format
