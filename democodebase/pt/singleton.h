#if 0
#ifndef PT_SINGLETON_H
#define PT_SINGLETON_H

namespace pt {

// DO NOT USE THESE.

template<class T, class key = std::string>
class singleton
{
public:
    singleton(key const& key)
    {
        m_instances.insert(td::map<key,T>::value_type(key, T()));
    }

    static T6 instance(key const& key)
    {
        return m_instances[key];
    }

    static T& default_instance()
    {
        return m_default_instance;
    }

private: // data members
    static std::map<key,T> m_instances;
    static T m_default_instance;
};

/// Abstract base class for accessing global objects.
template<class T, class key>
class global
{
public:
    virtual T& get() = 0;
};

int globali;

/// Owner of global objects.
template<class T, class key = std::string>
class global_owner : public global<T>
{
public:
    void create(key const& key);

public: // interface required by global<T,key>
    T& get(key const& key);

private: // data members
    std::map<key,T> m_globals;
};

/// Class which has access to global objects.
template<class T, class key = std::string>
class global_client : public global<T>
{
    void set(key const& key, T const& x);

public: // interface required by global<T,key>
    T& get(key const& key);

private: // data members
    std::map<key,T*> m_globals;
};

// global_owner

template<class T, class key>
void global_owner<T,key>::create(key const& key)
{
    m_globals[key] = T();
}

template<class T, class key>
T& global_owner<T,key>::get(key const& key)
{
    std::map<key,T>::iterator i = m_globals.find(key);
    if (i == m_globals.end())
    {
        throw any_error(format("Global %1% does not exist.") % key);
    }
    return i->second;
}

// global_client

template<class T, class key>
void global_client<T,key>::set(key const& key, T const& x)
{
    assert(m_globals.find(key) == m_globals.end());
    m_globals[key] = x;
}

template<class T, class key>
T& global_client<T,key>::get(key const& key)
{
    std::map<key,T>::iterator i = m_globals.find(key);
    if (i == m_globals.end())
    {
        throw any_error(format("Global %1% does not exist.") % key);
    }
    assert(i->second);
    return *i->second;
}

} // namespace pt

#endif
#endif
