#ifndef PT_P3D_FORMAT_POLYGON_H
#define PT_P3D_FORMAT_POLYGON_H

#include "pt/std/vector.h"

namespace p3d_format {

class corner_access;

/// Polygon with any number of corners. Polygon has unique corners which are
/// connected to shared points.
class polygon
{
public:
    polygon();
    polygon(polygon&& other);
    ~polygon();

    void add_corner(corner_access& corner);

    size_t corner_count() const;
    corner_access& corner_at(size_t index);
    corner_access const& corner_at(size_t index) const;

private: // data members
    std::vector<corner_access> m_corners;
};

} // p3d_format

#endif
