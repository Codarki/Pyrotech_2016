#include "basic_geometry.h"
#include "basic_drawable.h"

namespace pt { namespace d3d9 { namespace rendering {

basic_geometry::basic_geometry(
    std::shared_ptr<basic_drawable> drawable,
    std::shared_ptr<rendering::surface> surface)
: m_drawable(drawable)
, m_surface(surface)
{
}

box3f const& basic_geometry::bounding_box() const
{
    return m_bounding_box;
}

rendering::drawable& basic_geometry::drawable()
{
    return *m_drawable;
}

rendering::drawable const& basic_geometry::drawable() const
{
    return *m_drawable;
}

rendering::surface const& basic_geometry::surface() const
{
    return *m_surface;
}

rendering::surface& basic_geometry::surface()
{
    return *m_surface;
}


}}} // namespace pt::d3d9::rendering
