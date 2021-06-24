#ifndef PT_ANIMATABLE_VALUE_INL
#define PT_ANIMATABLE_VALUE_INL

namespace pt {

// Default constructor exists only to enable 2-phase construction.
template<typename T>
animatable_value<T>::animatable_value()
{
}

template<typename T>
animatable_value<T>::animatable_value(
    std::shared_ptr<animatable_value_source<T> > source)
:   m_source(source)
{
    assert(m_source);
}

template<typename T>
animatable_value<T>::animatable_value(animatable_value&& other)
: m_source(move(other.m_source))
{
}

template<typename T>
animatable_value<T> const& animatable_value<T>::operator=(
    animatable_value<T> const& other)
{
    assert(other.m_source);
    m_source = other.m_source;
    return *this;
}

template<typename T>
animatable_value<T> const& animatable_value<T>::operator=(
    animatable_value<T>&& other)
{
    assert(other.m_source);
    m_source = move(other.m_source);
    return *this;
}

template<typename T>
T animatable_value<T>::value_at(float time) const
{
    assert(m_source);
    return m_source->value_at(time);
}

//template<typename T>
//void animatable_value<T>::set_source(
//    std::shared_ptr<animatable_value_source<T> > source)
//{
//    m_source = source;
//}

template<typename T>
std::shared_ptr<animatable_value_source<T> > const& animatable_value<T>::source()
{
    return m_source;
}

template<typename T>
std::shared_ptr<animatable_value_source<T> > const& animatable_value<T>::source() const
{
    return m_source;
}

template<typename T>
inline bool animatable_value<T>::is_constant() const
{
    assert(m_source);
    return m_source->is_constant();
}

template<typename T>
inline string_wrapper animatable_value<T>::type_name() const
{
    assert(m_source);
    return m_source->type_name();
}

} // namespace pt

#endif
