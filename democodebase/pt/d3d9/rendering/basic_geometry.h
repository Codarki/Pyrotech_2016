#ifndef PT_D3D9_RENDERING_BASIC_GEOMETRY_H
#define PT_D3D9_RENDERING_BASIC_GEOMETRY_H

#include "geometry.h"
#include "pt/box.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class basic_drawable;

class basic_geometry : public geometry
{
public:
	basic_geometry(std::shared_ptr<basic_drawable> drawable,
        std::shared_ptr<rendering::surface> surface);

public: // interface required by geometry
    box3f const& bounding_box() const;
    rendering::drawable& drawable();
    rendering::drawable const& drawable() const;
    rendering::surface const& surface() const;
    rendering::surface& surface();

private: // data members
	std::shared_ptr<basic_drawable> m_drawable;
    std::shared_ptr<rendering::surface> m_surface;
    box3f m_bounding_box;
};

}}} // namespace pt::d3d9::rendering

#endif
