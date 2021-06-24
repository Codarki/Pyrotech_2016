#pragma once
#ifndef PT_D3D9_DETAIL_NATIVE_RESOURCES_H
#define PT_D3D9_DETAIL_NATIVE_RESOURCES_H

#include "pt/d3d9/native.h"
#include "pt/com_ptr.h"

namespace pt { namespace d3d9 { namespace detail {

template<typename T>
class native_interface
{
public:
    native_interface(com_ptr<T> const& ptr)
    : m_ptr(ptr)
    {
    }

    T* native_ptr()
    {
        return m_ptr.get();
    }

private: // data members
    com_ptr<T> m_ptr;
};

class native_texture_2d : public native_interface<IDirect3DTexture9>
{
public:
    native_texture_2d(com_ptr<IDirect3DTexture9> const& ptr)
    : native_interface(ptr)
    {
    }
};

}}} // namespace pt::d3d9::detail

#endif
