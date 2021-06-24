#include "point.h"
#include "corner_access.h"

namespace p3d_format {

// point

point::point()
{
}

point::point(point&& other)
: m_corners(static_cast<corner_collection&&>(other.m_corners)) // move
{
}

point::~point()
{
}

corner_access point::create_corner()
{
    size_t index = m_corners.create_corner(*this);
    return corner_access(m_corners, index);
}

size_t point::corner_count() const
{
    return m_corners.corner_count();
}

corner& point::corner_at(size_t index)
{
    return m_corners.corner_at(index);
}

corner const& point::corner_at(size_t index) const
{
    return m_corners.corner_at(index);
}

} // namespace p3d_format
