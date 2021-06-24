#ifndef PT_D3D9_UTILITY_H
#define PT_D3D9_UTILITY_H

#include "native.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 {

inline matrix4x4f convert(D3DMATRIX const& m)
{
    matrix4x4f result(matrix4x4f::uninitialized);
    result.set_row(0, vector4f(m._11, m._12, m._13, m._14));
    result.set_row(1, vector4f(m._21, m._22, m._23, m._24));
    result.set_row(2, vector4f(m._31, m._32, m._33, m._34));
    result.set_row(3, vector4f(m._41, m._42, m._43, m._44));
    return result;
}

inline D3DMATRIX convert(matrix4x4f const& m)
{
    return D3DXMATRIX(&m(0, 0));
}

}} // namespace pt::d3d9

#endif
