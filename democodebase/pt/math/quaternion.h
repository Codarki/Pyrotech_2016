#ifndef PT_MATH_QUATERNION_H
#define PT_MATH_QUATERNION_H

//#include "vector.h"
#include <boost/math/quaternion.hpp>


#include "pt/win32.h"
#include <d3dx9math.h>

namespace pt { namespace math {

//template<typename T>
//class quaternion : public ::boost::math::quaternion<T>
//{
//public:
//    quaternion()
//    {
//    }
//    explicit quaternion(vector<T,4> const& v)
//    :   ::boost::math::quaternion<T>(v[0], v[1], v[2], v[3])
//    {
//    }
//    explicit quaternion(T a, T b, T c, T d)
//    :   ::boost::math::quaternion<T>(a, b, c, d)
//    {
//    }
//};
//
//typedef quaternion<float> quaternionf;

typedef ::boost::math::quaternion<float> quaternionf;

// Random tidbits:

// A number of the form a + 0i + 0j + 0k, where a is a real number, is called
// real, and a number of the form 0 + bi + cj + dk, where b, c, and d are
// real numbers, is called pure imaginary.

// q0*q1 = q1*q0 if and only if v0 x v1 = 0 (these two vectors are parallel).

// The inverse of a unit quaternion and the product of unit quaternions are
// themselves unit quaternions.

// Conjugation can be used to extract the scalar and vector parts of a
// quaternion. Denoted by q*. The scalar part of q is (q + q*)/2, and the
// vector part of q is (q - q*)/2.

D3DXQUATERNION conjugate(D3DXQUATERNION const& q);

// A unit quaternion is a quaternion of norm one. Multiplying a quaternion by
// a real number scales its norm by the absolute value of the number.
float norm(D3DXQUATERNION const& q);

// The multiplicative inverse of a quaternion q.
// The product of a quaternion with its reciprocal should equal 1.
D3DXQUATERNION reciprocal(D3DXQUATERNION const& q);

// Returns unit quaternion.
// Dividing a non-zero quaternion q by its norm produces a unit quaternion.
D3DXQUATERNION normalize(D3DXQUATERNION const& q);

// Returns true if \a q is an unit quaternion.
bool is_unit_length(D3DXQUATERNION const& q);
// Returns true if w is 0.
bool is_pure_imaginary(D3DXQUATERNION const& q);

D3DXQUATERNION lerp(D3DXQUATERNION const& q1, D3DXQUATERNION const& q2,
    float t);

// Although q2 and -q2 represent the same rotation, the values of
// slerp(q1, q2, t) and slerp(q1, -q2, t) are not the same. It is customary
// to choose the sign on q2 so that dot(q1, q2) >= 0. This choice avoids
// extra spinning caused by the interpolated rotations.
D3DXQUATERNION slerp(D3DXQUATERNION const& q1, D3DXQUATERNION const& q2,
    float t);

}} // namespace pt::math

namespace pt {

inline std::string to_string(pt::math::quaternionf const&)
{
    // TODO
    return std::string("quaternionf");
}

} // namespace pt

#endif
