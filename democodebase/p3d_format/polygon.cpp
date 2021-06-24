#include "polygon.h"
#include "corner.h"
#include "corner_access.h"
#include "pt/boost/assert.h"

namespace p3d_format {

// polygon

polygon::polygon()
{
}

polygon::polygon(polygon&& other)
: m_corners(static_cast<std::vector<corner_access>&&>(other.m_corners)) // move
{
}

polygon::~polygon()
{
}

void polygon::add_corner(corner_access& corner)
{
    if (m_corners.empty())
        m_corners.reserve(3);

    corner.get().set_polygon(this);
    m_corners.push_back(corner);
}

size_t polygon::corner_count() const
{
    return m_corners.size();
}

corner_access& polygon::corner_at(size_t index)
{
    assert(index < corner_count());
    return m_corners[index];
}

corner_access const& polygon::corner_at(size_t index) const
{
    assert(index < corner_count());
    return m_corners[index];
}

} // namespace p3d_format
