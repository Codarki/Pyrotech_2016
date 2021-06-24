#ifndef PT_D3D9_RENDERING_SIMPLE_ARRAY_SETTER_H
#define PT_D3D9_RENDERING_SIMPLE_ARRAY_SETTER_H

#include "constant_setter.h"

namespace pt { namespace d3d9 { namespace rendering {

template<typename T, size_t C>
class simple_array_setter : public constant_setter
{
public:
    void set_value(size_t index, T const& value);

public: // interface required by variable_setter
    void update_target(float time) const;

private:
    T m_value[C];
};

template<typename T, size_t C>
inline void simple_array_setter<T,C>::set_value(size_t index, T const& value)
{
    assert(index < C);
    m_value[index] = value;
}

template<typename T, size_t C>
inline void simple_array_setter<T,C>::update_target(float) const
{
    set_constant(constant(), constants(), m_value, C);
}

}}} // namespace pt::d3d9::rendering

#endif
