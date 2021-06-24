#include "quaternion.h"
#include "../vector.h"

namespace pt { namespace math {

// quaternions, like the rotations or other linear transforms, are not
// "handed" (as in left-handed vs right-handed). 

D3DXQUATERNION quaternion_from_axis_angle(vector3f const& axis, float angle)
{
    vector3f v = axis * std::sin(angle / 2.0f);
    float w = std::cos(angle / 2.0f);
    return D3DXQUATERNION(v[0], v[1], v[2], w);
}

//vector4f to_axis_angle(D3DXQUATERNION const& quat)
//{
//    float angle = 2 * std::acos(quat.w);
//
//    vector3f v = axis * std::sin(angle / 2.0f);
//    float w = std::cos(angle / 2.0f);
//    return D3DXQUATERNION(v[0], v[1], v[2], w);
//}

D3DXQUATERNION conjugate(D3DXQUATERNION const& quat)
{
    return D3DXQUATERNION(-quat.x, -quat.y, -quat.z, quat.w);
}

// (there was another version if this function without sqrt)
float norm(D3DXQUATERNION const& quat)
{
    return std::sqrt(
        quat.x * quat.x
        + quat.y * quat.y
        + quat.z * quat.z
        + quat.w * quat.w);
}

D3DXQUATERNION reciprocal(D3DXQUATERNION const& quat)
{
    float n = norm(quat);
    return conjugate(quat) / (n * n);
}

D3DXQUATERNION normalize(D3DXQUATERNION const& quat)
{
    float n = norm(quat);
    return quat / n;
}

bool is_unit_length(D3DXQUATERNION const& quat)
{
    float const epsilon = 0.00001f;
    return std::abs(1.0f - norm(quat)) < epsilon;
}

bool is_pure_imaginary(D3DXQUATERNION const& quat)
{
    float const epsilon = 0.00001f;
    return std::abs(quat.w) < epsilon;
}

D3DXQUATERNION lerp(D3DXQUATERNION const& q1, D3DXQUATERNION const& q2,
    float t)
{
    return q1 + t * (q2 - q1);
}

D3DXQUATERNION slerp(D3DXQUATERNION const& q1, D3DXQUATERNION const& q2,
    float t)
{
    float const epsilon = 0.00001f;

    //if (t <= 0.0f)
    //    return q1;
    //if (t >= 1.0f)
    //    return q2;

    D3DXQUATERNION q3 = q2;

    float c = D3DXQuaternionDot(&q1, &q3);

    //if (c < 0.0f)
    //{
    //    q3 = -q3;
    //    c = -c;
    //}

    if (c > 1.0f - epsilon)
        return normalize(lerp(q1, q3, t));

    // acos() clamps input to [-1, 1]
    float a = std::acos(c);

    D3DXQUATERNION tmp1 = std::sin((1.0f - t) * a) * q1;
    D3DXQUATERNION tmp2 = std::sin(t * a) * q3;

    return (tmp1 + tmp2) / std::sin(a);
}

}} // namespace pt::math
