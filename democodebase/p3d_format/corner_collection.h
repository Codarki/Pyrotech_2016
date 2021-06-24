#ifndef PT_P3D_FORMAT_CORNER_COLLECTION_H
#define PT_P3D_FORMAT_CORNER_COLLECTION_H

#include "corner.h"
#include "pt/std/vector.h"

namespace p3d_format {

class corner_collection
{
public:
    corner_collection();
    corner_collection(corner_collection&& other);
    ~corner_collection();

    size_t create_corner(p3d_format::point& point);
    size_t corner_count() const;
    corner& corner_at(size_t index);
    corner const& corner_at(size_t index) const;

private: // data members
    std::vector<corner> m_corners;
};

} // p3d_format

#endif
