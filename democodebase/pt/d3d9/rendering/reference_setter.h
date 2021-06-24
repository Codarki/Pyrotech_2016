#pragma once
#ifndef PT_D3D9_RENDERING_REFERENCE_SETTER_H
#define PT_D3D9_RENDERING_REFERENCE_SETTER_H

#include "constant_setter.h"
#include "pt/d3d9/constant_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

template<typename T>
class reference_setter : public constant_setter
{
public:
    reference_setter(T const& source);

public: // interface required by variable_setter
    void update_target(float time) const;

private: // nonassignable
    reference_setter const& operator=(reference_setter const&);

private: // data members
    T const& m_source;
};

// .inl

template<typename T>
reference_setter<T>::reference_setter(T const& source)
:   m_source(source)
{
}

template<typename T>
void reference_setter<T>::update_target(float) const
{
    //set_constant(constants(), constant().handle(), m_source);
    set_constant(constant(), constants(), m_source);
}

}}} // namespace pt::d3d9::rendering

#endif
