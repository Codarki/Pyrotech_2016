#include "corner_collection.h"
#include "pt/std/cassert.h"

namespace p3d_format {

// corner_collection

corner_collection::corner_collection()
{
}

corner_collection::corner_collection(corner_collection&& other)
: m_corners(static_cast<std::vector<corner>&&>(other.m_corners)) // move
{
}

corner_collection::~corner_collection()
{
}

size_t corner_collection::create_corner(p3d_format::point& point)
{
    if (m_corners.empty())
        m_corners.reserve(6);

    m_corners.push_back(corner(&point));
    return m_corners.size() - 1;
}

size_t corner_collection::corner_count() const
{
    return m_corners.size();
}

corner& corner_collection::corner_at(size_t index)
{
    assert(index < corner_count());
    return m_corners[index];
}

corner const& corner_collection::corner_at(size_t index) const
{
    assert(index < corner_count());
    return m_corners[index];
}

} // namespace p3d_format
