#ifndef PT_D3D9_RENDERING_ANIMATABLE_SETTER_INL
#define PT_D3D9_RENDERING_ANIMATABLE_SETTER_INL

namespace pt { namespace d3d9 { namespace rendering {

template<typename T>
inline animatable_setter<T>::animatable_setter(
    std::shared_ptr<animatable_value<T> > value)
:   m_value(value)
{
}

template<typename T>
inline void animatable_setter<T>::update_target(float time) const
{
    set_constant(constants(), constant().handle(), m_value->value_at(time));
}

}}} // namespace pt::d3d9::rendering

#endif
