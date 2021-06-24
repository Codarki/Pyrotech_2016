#pragma once
#ifndef PT_D3D9_RENDERING_GEOMETRY_H
#define PT_D3D9_RENDERING_GEOMETRY_H

#include "pt/box_fwd.h"

namespace pt { namespace d3d9 { namespace rendering {

class drawable;
class surface;

class geometry
{
public: // interface which subclasses must implement
    /// Bounding box in local space.
    virtual box3f const& bounding_box() const = 0;

    /// Drawable for drawing this geometry.
    virtual rendering::drawable& drawable() = 0;
    virtual rendering::drawable const& drawable() const = 0;

    virtual rendering::surface const& surface() const = 0;
    virtual rendering::surface& surface() = 0;
};

}}} // namespace pt::d3d9::rendering

#endif
