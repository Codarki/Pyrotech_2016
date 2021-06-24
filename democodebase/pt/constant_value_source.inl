#ifndef PT_CONSTANT_VALUE_SOURCE_INL
#define PT_CONSTANT_VALUE_SOURCE_INL

namespace pt {

template<typename T>
inline constant_value_source<T>::operator T const&()
{
    return m_value;
}

template<typename T>
inline constant_value_source<T>::constant_value_source(T value)
:   m_value(value)
{
}

template<typename T>
inline bool constant_value_source<T>::is_constant() const
{
    return true;
}

template<typename T>
inline T constant_value_source<T>::value_at(float) const
{
    return m_value;
}

template<typename T>
string_wrapper constant_value_source<T>::type_name() const
{
    return "constant " + std::string(typeid(m_value).name()) + " : " + pt::to_string(m_value); 
}

} // namespace pt

#endif
