#ifndef PT_D3D9_RENDERING_CONSTANT_SETTER_UTILITY_H
#define PT_D3D9_RENDERING_CONSTANT_SETTER_UTILITY_H

#include "reference_setter.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/debug_only.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

template<typename T>
std::shared_ptr<reference_setter<T> > create_reference_setter(T const& ref,
    constant const& c);

template<typename T>
std::unique_ptr<reference_setter<T> > create_unique_reference_setter(T const& ref,
    constant const& c);

template<>
inline std::shared_ptr<reference_setter<matrix4x4f> > create_reference_setter(
    matrix4x4f const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_FLOAT);
    assert(c.desc().Class == D3DXPC_MATRIX_COLUMNS);
    assert(c.desc().Rows == 4);
    assert(c.desc().Columns == 4);

    return std::make_shared<reference_setter<matrix4x4f>>(ref);
}

template<>
inline std::unique_ptr<reference_setter<matrix4x4f> > create_unique_reference_setter(
    matrix4x4f const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_FLOAT);
    assert(c.desc().Class == D3DXPC_MATRIX_COLUMNS);
    assert(c.desc().Rows == 4);
    assert(c.desc().Columns == 4);

    return std::unique_ptr<reference_setter<matrix4x4f> >(
        new reference_setter<matrix4x4f>(ref));
}

template<>
inline std::shared_ptr<reference_setter<vector3f> > create_reference_setter(
    vector3f const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_FLOAT);
    assert(c.desc().Class == D3DXPC_VECTOR);
    assert(c.desc().Rows == 1);
    assert(c.desc().Columns == 3);

    return std::make_shared<reference_setter<vector3f>>(ref);
}

template<>
inline std::unique_ptr<reference_setter<vector3f> > create_unique_reference_setter(
    vector3f const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_FLOAT);
    assert(c.desc().Class == D3DXPC_VECTOR);
    assert(c.desc().Rows == 1);
    assert(c.desc().Columns == 3);

    return std::unique_ptr<reference_setter<vector3f> >(
        new reference_setter<vector3f>(ref));
}

template<>
inline std::shared_ptr<reference_setter<float> > create_reference_setter(
    float const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_FLOAT);
    assert(c.desc().Class == D3DXPC_SCALAR);
    assert(c.desc().Rows == 1);
    assert(c.desc().Columns == 1);

    return std::make_shared<reference_setter<float>>(ref);
}

template<>
inline std::unique_ptr<reference_setter<float> > create_unique_reference_setter(
    float const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_FLOAT);
    assert(c.desc().Class == D3DXPC_SCALAR);
    assert(c.desc().Rows == 1);
    assert(c.desc().Columns == 1);

    return std::unique_ptr<reference_setter<float> >(
        new reference_setter<float>(ref));
}

template<>
inline std::unique_ptr<reference_setter<bool> > create_unique_reference_setter(
    bool const& ref, constant const& PT_DEBUG_ONLY(c))
{
    assert(c.desc().Type == D3DXPT_BOOL);
    assert(c.desc().Class == D3DXPC_SCALAR);
    assert(c.desc().Rows == 1);
    assert(c.desc().Columns == 1);

    return std::unique_ptr<reference_setter<bool> >(
        new reference_setter<bool>(ref));
}

}}} // namespace pt::d3d9::rendering

#endif
