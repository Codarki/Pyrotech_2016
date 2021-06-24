#ifndef PT_GLOBAL_INL
#define PT_GLOBAL_INL

namespace pt {

template<class T, class key>
global<T,key>::~global()
{
}

// global_owner

template<class T, class key>
void global_owner<T,key>::create(key const& key)
{
    m_globals[key] = T();
}

template<class T, class key>
T& global_owner<T,key>::instance(key const& key)
{
    std::map<key,T>::iterator i = m_globals.find(key);
    if (i == m_globals.end())
    {
        throw any_error(format("Global %1% does not exist.") % key);
    }
    return i->second;
}

template<class T, class key>
T const& global_owner<T,key>::instance(key const& key) const
{
    std::map<key,T>::const_iterator i = m_globals.find(key);
    if (i == m_globals.end())
    {
        throw any_error(format("Global %1% does not exist.") % key);
    }
    return i->second;
}

template<class T, class key>
size_t global_owner<T,key>::count() const
{
    return m_globals.size();
}

template<class T, class key>
std::pair<key const&, T*> global_owner<T,key>::at(size_t index) const
{
    std::map<key,T>::iterator i = m_globals.find(key);
    assert(i != m_globals.end());

    key const& key = i->first;
    T* value = &(i->second);

    std::pair<key const&, T*> result(key, tmp);
    return result;
}

// global_client

template<class T, class key>
global_client<T,key>::global_client(global<T,key> const& x)
{
    for (size_t i = 0; i < x.count(); ++i)
    {
        std::pair<key const&, T*> tmp = x.at(i);
        set(tmp.first, tmp.second);
    }
}

template<class T, class key>
void global_client<T,key>::set(key const& key, T const& x)
{
    assert(m_globals.find(key) == m_globals.end());
    m_globals[key] = x;
}

template<class T, class key>
T& global_client<T,key>::instance(key const& key)
{
    std::map<key,T>::iterator i = m_globals.find(key);
    if (i == m_globals.end())
    {
        throw any_error(format("Global %1% does not exist.") % key);
    }
    assert(i->second);
    return *i->second;
}

template<class T, class key>
T const& global_client<T,key>::instance(key const& key) const
{
    std::map<key,T>::const_iterator i = m_globals.find(key);
    if (i == m_globals.end())
    {
        throw any_error(format("Global %1% does not exist.") % key);
    }
    assert(i->second);
    return *i->second;
}

template<class T, class key>
size_t global_client<T,key>::count() const
{
    return m_globals.size();
}

template<class T, class key>
std::pair<key const&, T*> global_client<T,key>::at(size_t index) const
{
    std::map<key,T>::iterator i = m_globals.find(key);
    assert(i != m_globals.end());

    key const& key = i->first;
    T* value = i->second;

    std::pair<key const&, T*> result(key, tmp);
    return result;
}

} // namespace pt

#endif
