#ifndef PT_D3D9_RENDERING_CONSTANT_SETTER_INL
#define PT_D3D9_RENDERING_CONSTANT_SETTER_INL

namespace pt { namespace d3d9 { namespace rendering {

//template<typename T>
//inline constant_setter<T>::constant_setter(constant_buffer const& buffer,
//    constant const& constant, animatable_value<T> value)
//:   m_constant(constant)
//,   m_buffer(buffer)
//,   m_value(value)
//{
//}
//
//template<typename T>
//void associate_to(constant_buffer const& buffer, constant const& constant)
//{
//}
//
//template<typename T>
//inline void constant_setter<T>::set_source(animatable_value<T> value)
//{
//    m_value = value;
//}
//
//template<typename T>
//inline void constant_setter<T>::update_target(float time) const
//{
//    set<T>(m_buffer, m_constant.handle(), m_value.value_at(time));
//}

//template<>
//inline void constant_setter<vector4f>::update_target(float) const
//{
//    set(m_buffer, m_constant.handle(), vector4f(0.0f, 1.0f, 0.1f, 1.0f));
//}
//
//template<>
//inline void constant_setter<matrix4x4f>::update_target(float) const
//{
//    set(m_buffer, m_constant.handle(), matrix4x4f());
//}

}}} // namespace pt::d3d9::rendering

#endif
