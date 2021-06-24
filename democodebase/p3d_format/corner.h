#ifndef PT_P3D_FORMAT_CORNER_H
#define PT_P3D_FORMAT_CORNER_H

namespace p3d_format {

class point;
class polygon;

/// Polygon corner.
///
/// Corner shares the position of a point, but provides distinction for
/// different polygons attached to that point. This distiction can be a
/// different vertex normal for example. Corner is a weak class, and is only
/// kept alive by point and polygon.
class corner
{
public:
    corner(p3d_format::point* point);

    p3d_format::point const* point() const;
    p3d_format::polygon const* polygon() const;

private:
    friend p3d_format::polygon;
    void set_polygon(p3d_format::polygon* polygon);

private: // data members
    p3d_format::point* m_point;
    p3d_format::polygon* m_polygon;
};

} // p3d_format

#endif
