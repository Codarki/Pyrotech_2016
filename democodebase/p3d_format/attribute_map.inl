#ifndef PT_P3D_FORMAT_ATTRIBUTE_MAP_INL
#define PT_P3D_FORMAT_ATTRIBUTE_MAP_INL

namespace p3d_format {

#if 0

template<typename T, typename Value>
inline void attribute_map<T,Value>::add(T const& key, Value const& attribute)
{
    std::pair<container::iterator, bool> result
        = m_attributes.insert(container::value_type(key, attribute));
    assert(result.second);
}

template<typename T, typename Value>
inline void attribute_map<T,Value>::add(T const& key, Value&& attribute)
{
    std::pair<container::iterator, bool> result
        = m_attributes.insert(container::value_type(key, std::move(attribute)));
    assert(result.second);
}

template<typename T, typename Value>
inline Value& attribute_map<T,Value>::value_for(T const& key)
{
    container::iterator i = m_attributes.find(key);
    assert(i != m_attributes.end());
    return i->second;
}

template<typename T, typename Value>
inline Value const& attribute_map<T,Value>::value_for(T const& key) const
{
    container::const_iterator i = find(key);
    assert(i != m_attributes.end());
    return i->second;
}

template<typename T, typename Value>
inline typename attribute_map<T,Value>::const_iterator
    attribute_map<T,Value>::find(T const& key) const
{
    return m_attributes.find(key);
}

template<typename T, typename Value>
inline typename attribute_map<T,Value>::const_iterator
    attribute_map<T,Value>::begin() const
{
    return m_attributes.begin();
}

template<typename T, typename Value>
inline typename attribute_map<T,Value>::const_iterator
    attribute_map<T,Value>::end() const
{
    return m_attributes.end();
}

template<typename T, typename Value>
inline typename attribute_map<T,Value>::iterator
    attribute_map<T,Value>::begin()
{
    return m_attributes.begin();
}

template<typename T, typename Value>
inline typename attribute_map<T,Value>::iterator
    attribute_map<T,Value>::end()
{
    return m_attributes.end();
}

template<typename T, typename Value>
inline bool attribute_map<T,Value>::empty() const
{
    return m_attributes.empty();
}

#else

template<typename T, typename Value>
void attribute_map<T,Value>::reserve(size_t count)
{
    m_attributes.reserve(count);
}

template<typename T, typename Value>
void attribute_map<T,Value>::begin_inserting()
{
    m_is_inserting = true;
}

template<typename T, typename Value>
void attribute_map<T,Value>::end_inserting()
{
    std::sort(m_attributes.begin(), m_attributes.end());
    m_is_inserting = false;
}

template<typename T, typename Value>
void attribute_map<T,Value>::add(T const& key, Value const& attribute)
{
    assert(m_is_inserting);
    m_attributes.push_back(entry(key, attribute));
}

template<typename T, typename Value>
void attribute_map<T,Value>::add(T const& key, Value&& attribute)
{
    assert(m_is_inserting);
    m_attributes.push_back(entry(key, static_cast<Value&&>(attribute)));
}

template<typename T, typename Value>
inline Value& attribute_map<T,Value>::value_for(T const& key)
{
    assert(!m_is_inserting);

    std::vector<entry>::iterator low
        = std::lower_bound(m_attributes.begin(), m_attributes.end(), entry(key));

    assert(low != m_attributes.end());
    return low->value;
}

template<typename T, typename Value>
inline Value const& attribute_map<T,Value>::value_for(T const& key) const
{
    assert(!m_is_inserting);

    std::vector<entry>::const_iterator low
        = std::lower_bound(m_attributes.begin(), m_attributes.end(), entry(key));

    assert(low != m_attributes.end());
    return low->value;
}

template<typename T, typename Value>
inline bool attribute_map<T,Value>::empty() const
{
    return m_attributes.empty();
}

#endif

} // namespace p3d_format

#endif
