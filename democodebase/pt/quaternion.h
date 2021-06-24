#ifndef PYROTECH_QUATERNION_H
#define PYROTECH_QUATERNION_H
#if 0
#include "vector.h"

namespace pt {

/// \todo Implementation not finished. Research if boost's quaternion is usable
/// (and fast).
class quaternion
{
public:
    quaternion();
    quaternion(float s, vector3f const& v);

    float s() const;
    vector3f const& v() const;

private: // data members
    float m_s;
    vector3f m_v;
};

// .inl

quaternion::quaternion()
:   m_s(0.0f)
,   m_v(0, 0, 0)
{
}

quaternion::quaternion(float s, vector3f const& v)
:   m_s(s)
,   m_v(v)
{
}

float quaternion::s() const
{
    return m_s;
}

vector3f const& quaternion::v() const
{
    return m_v;
}

// free functions

quaternion quaternion_from_axis(float angle, vector3f const& axis)
{
    float half_angle = angle / 2.0f;
    return quaternion(std::cos(half_angle), axis * std::sin(half_angle));
}

quaternion conjugate(quaternion const& x)
{
    return quaternion(x.s(), -x.v());
}

quaternion operator+(quaternion const& a, quaternion const& b)
{
    return quaternion(a.s() + b.s(), a.v() + b.v());
}

quaternion operator-(quaternion const& a, quaternion const& b)
{
    return quaternion(a.s() - b.s(), a.v() - b.v());
}

quaternion operator*(quaternion const& lhs, float rhs)
{
    return quaternion(lhs.s() * rhs, lhs.v() * rhs);
}

quaternion operator*(quaternion const& a, quaternion const& b)
{
    float s = a.s() * b.s() - dot(a.v(), b.v());
    vector3f v = a.s() * b.v() + b.s() * a.v() + cross(a.v(), b.v());
    return quaternion(s, v);
}

quaternion operator/(quaternion const& lhs, float rhs)
{
    return quaternion(lhs.s() / rhs, lhs.v() / rhs);
}

quaternion operator/(quaternion const& , quaternion const& )
{
    assert(false);
    return quaternion();
}

//quaternion operator+(quaternion const& other) const
//{
//    return quaternion(m_rabc + other.m_rabc);
//}
//
//quaternion operator*(quaternion const& other) const
//{
//    float r = m_rabc[0] * other.m_rabc[0] - m_rabc[1] * other.m_rabc[1]
//        - m_rabc[2] * other.m_rabc[2] - m_rabc[3] * other.m_rabc[3]
//
//    float a = m_rabc[0] * other.m_rabc[0] - m_rabc[1] * other.m_rabc[1]
//        - m_rabc[2] * other.m_rabc[2] - m_rabc[3] * other.m_rabc[3]
//
//    return quaternion(
//        );
//}
//
///*
//Q1 Q2 = 
//[ r1 r2 - a1 a2 - b1 b2 - c1 c2 ] +
//[ r1 a2 + a1 r2 + b1 c2 - c1 b2 ] i +
//[ r1 b2 + b1 r2 + c1 a2 - a1 c2 ] j +
//[ r1 c2 + c1 r2 + a1 b2 - b1 a2 ] k
//*/
//
//vector4f const& data() const
//{
//    return r_abc;
//}
//
//float congujate() const
//{
//    return m_rabc[0] - m_rabc[1] - m_rabc[2] - m_rabc[3];
//}
//
//quaternion operator+(quaternion const& a, quaternion const& b)
//{
//    return quaternion(a.data() + b.data());
//}

} // namespace pt
#endif
#endif
