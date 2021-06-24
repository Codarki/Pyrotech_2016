#pragma once
#ifndef PT_MATH_PLANE_H
#define PT_MATH_PLANE_H

#include "pt/vector.h"
#include "pt/matrix.h"
#include "pt/matrix_operations.h"

namespace pt { namespace math {

// The members of the Plane structure take the form of the general plane equation. They fit into the general plane equation so that Ax + By + Cz + Dw = 0.
// a*x + b*y + c*z + d = 0
class plane
{
public:
    plane()
    {
    }

    plane(vector3f normal, float d)
        : m_data(normal, d)
    {
    }

    vector3f normal() const
    {
        return vector3f(m_data[0], m_data[1], m_data[2]);
    }

    float d() const
    {
        return m_data[3];
    }

    float const* data() const
    {
        return m_data.data();
    }

    vector4f const& as_vector() const
    {
        return m_data;
    }

private:
    vector4f m_data;
};

inline plane make_plane(vector3f const& point, vector3f const& normal)
{
    auto normalized = normalize(normal);
    float d = -dot(point, normalized);
    return plane(normalized, d);
}

inline plane make_plane(vector3f const& point, vector3f const& direction1, vector3f const& direction2)
{
    vector3f normal = cross(direction1, direction2);
    return make_plane(point, normal);
}

inline plane make_plane_from_points(vector3f const& point0, vector3f const& point1, vector3f const& point2)
{
    auto normal = normalize(cross(point1 - point0, point2 - point0));
    return make_plane(point0, normal);
}

inline plane normalize(plane const& p)
{
    auto normal = p.normal();
    float distance = length(normal);
    normal = normal / distance;
    float d = p.d() / distance;
    return plane(normal, d);
}

inline float distance(plane const& plane, vector3f const& point)
{
    auto normal = plane.normal();
    return normal[0] * point[0] + normal[1] * point[1] + normal[2] * point[2] + plane.d();
}

inline vector3f closest_point(plane const& plane, vector3f const& point)
{
    return point - plane.normal() * distance(plane, point);
}

inline float dot_coord(plane const& plane, vector3f const& v)
{
    auto normal = plane.normal();
    return normal[0] * v[0] + normal[1] * v[1] + normal[2] * v[2] + plane.d();
}

inline float dot_normal(plane const& plane, vector3f const& v)
{
    auto normal = plane.normal();
    return normal[0] * v[0] + normal[1] * v[1] + normal[2] * v[2];
}

inline matrix4x4f reflect(plane const& plane_)
{
    // P = normalize(plane);
    // - 2 * P.a * P.a + 1     - 2 * P.b * P.a         - 2 * P.c * P.a         0
    // - 2 * P.a * P.b         - 2 * P.b * P.b + 1     - 2 * P.c * P.b         0
    // - 2 * P.a * P.c         - 2 * P.b * P.c         - 2 * P.c * P.c + 1     0
    // - 2 * P.a * P.d         - 2 * P.b * P.d         - 2 * P.c * P.d         1

    plane p = normalize(plane_);

    float a = p.normal()[0];
    float b = p.normal()[1];
    float c = p.normal()[2];
    float d = p.d();

    matrix4x4f m(matrix4x4f::uninitialized);

    m(0, 0) = -2.0f * a * a + 1;
    m(1, 0) = -2.0f * a * b;
    m(2, 0) = -2.0f * a * c;
    m(3, 0) = -2.0f * a * d;

    m(0, 1) = -2.0f * b * a;
    m(1, 1) = -2.0f * b * b + 1;
    m(2, 1) = -2.0f * b * c;
    m(3, 1) = -2.0f * b * d;

    m(0, 2) = -2.0f * c * a;
    m(1, 2) = -2.0f * c * b;
    m(2, 2) = -2.0f * c * c + 1;
    m(3, 2) = -2.0f * c * d;

    //m.set_row(3, vector4f(0, 0, 0, 1));
    m.set_column(3, vector4f(0, 0, 0, 1));

    return m;
}

inline plane transform(plane const& p, matrix4x4f const& m)
{
    //_pos = trans * _pos;
    //_direction = trans * _direction - trans * Vector0;
    //_d = -(_pos*_direction);

    //plane p2 = normalize(p);
    //vector3f origo = p2.normal() * p2.d();
    //vector3f normal = p2.normal();
    //origo = transform_position(origo, m);
    //auto m2 = transpose(invert(m));
    //normal = transform_direction(normal, m2) - transform_direction(vector3f(), m2);
    //float d = -dot(origo, normal);
    //return normalize(plane(normal, d));

    /////
    vector3f origo = p.normal() * p.d();
    vector3f normal = p.normal();

    assert(length(normal) > 0.99f && length(normal) < 1.01f);

    auto m2 = transpose(invert(m));

    origo = transform_position_w(origo, m);

    normal = transform_direction(normal, m2);
    //normal = transform_direction(normal, m2);
    ////normal = transform_direction(normal, transpose(m2));

    ////float d = dot(origo, normal);
    ////return plane(normal, d);

    return make_plane(origo, normal);
}

}}

#endif
