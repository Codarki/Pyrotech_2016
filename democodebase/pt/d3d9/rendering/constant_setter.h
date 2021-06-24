#pragma once
#ifndef PT_D3D9_RENDERING_CONSTANT_SETTER_H
#define PT_D3D9_RENDERING_CONSTANT_SETTER_H

#include "variable_setter.h"
#include "pt/d3d9/constant_buffer_fwd.h"

namespace pt { namespace d3d9 {
    class constant;
}}

namespace pt { namespace d3d9 { namespace rendering {

/// Abstract base class for setting shader constants.
class constant_setter : public variable_setter
{
public:
    constant_setter();

    void associate_to(constant_buffer const& buffer,
        d3d9::constant const& constant);

    d3d9::constant const& constant() const;

    constant_setter(constant_setter const&) = delete;
    constant_setter& operator=(constant_setter const&) = delete;

protected:
    constant_buffer const& constants() const;

private:
    d3d9::constant const* m_constant;
    constant_buffer const* m_buffer;
};

}}} // namespace pt::d3d9::rendering

//#include "constant_setter.inl"

#endif
