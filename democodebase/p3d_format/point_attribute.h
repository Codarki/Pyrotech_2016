#ifndef PT_P3D_FORMAT_POINT_ATTRIBUTE_H
#define PT_P3D_FORMAT_POINT_ATTRIBUTE_H

#include "attribute_map.h"
#include "corner_access.h"

namespace p3d_format {

class point;
class polygon;

/// Container of attributes mapped to corners.
template<typename T>
class corner_attribute : public attribute_map<corner_access,T>
{
};

/// Container of attributes mapped to points.
template<typename T>
class point_attribute : public attribute_map<point const*,T>
{
};

/// Container of attributes mapped to polygons.
template<typename T>
class polygon_attribute : public attribute_map<polygon const*,T>
{
};

} // namespace p3d_format

#endif
