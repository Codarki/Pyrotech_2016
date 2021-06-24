#ifndef PT_D3D9_RENDERING_SIMPLE_CONSTANT_SETTER_H
#define PT_D3D9_RENDERING_SIMPLE_CONSTANT_SETTER_H

#include "constant_setter.h"

namespace pt { namespace d3d9 { namespace rendering {

template<typename T>
class simple_constant_setter : public constant_setter
{
public:
    simple_constant_setter(T value);

    void set_value(T const& value);

public: // interface required by variable_setter
    void update_target(float time) const;

private:
    T m_value;
};

template<typename T>
inline simple_constant_setter<T>::simple_constant_setter(T value)
:   m_value(value)
{
}

template<typename T>
inline void simple_constant_setter<T>::set_value(T const& value)
{
    m_value = value;
}

template<typename T>
inline void simple_constant_setter<T>::update_target(float) const
{
    set_constant(constants(), constant().handle(), m_value);
}

}}} // namespace pt::d3d9::rendering

#endif
