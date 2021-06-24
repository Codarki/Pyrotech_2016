#ifndef PT_P3D_FORMAT_POINT_H
#define PT_P3D_FORMAT_POINT_H

#include "corner_collection.h"

namespace p3d_format {

class corner_access;

/// Point where multiple corners are attached to.
class point
{
public:
    point();
    point(point&& other);
    ~point();

    corner_access create_corner();

    size_t corner_count() const;
    corner& corner_at(size_t index);
    corner const& corner_at(size_t index) const;

private: // data members
    corner_collection m_corners;
    //std::vector<corner> m_corners;
};

} // p3d_format

#endif
