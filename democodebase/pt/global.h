#ifndef PT_GLOBAL_H
#define PT_GLOBAL_H

namespace pt {

//int example = global<int>::get_instance("my_global");

/// Abstract base class for accessing global objects.
/// NOTE. this framework is not implemented fully yet.
template<class T, class key = std::string>
class global
{
public:
    virtual ~global();

    /// Returns collection of global T's. User is responsible for the
    /// definition. Main projects usually return global_owner, and client
    /// projects, such as dlls return global_client.
    static global& get();

    /// Little helper.
    static T& get_instance(key const& key)
    {
        return get().instance(key);
    }

public: // interface which subclasses must implement
    /// Returns global instance of type T identified by \a key.
    virtual T& instance(key const& key) = 0;
    virtual T const& instance(key const& key) const = 0;

protected: // interface which subclasses must implement
    // this interface is only used for constructing global_client.
    virtual size_t count() const = 0;
    virtual std::pair<key const&, T*> at(size_t index) const = 0;
};

/// Owner of global objects. There usually exists only single object of this
/// class in a project. All others should be objects of global_client<T,key>.
template<class T, class key = std::string>
class global_owner : public global<T>
{
public:
    /// Default constructs instance of T.
    void create(key const& key);

public: // interface required by global<T,key>
    T& instance(key const& key);
    T const& instance(key const& key) const;

protected: // interface required by global<T,key>
    size_t count() const;
    std::pair<key const&, T*> at(size_t index) const;

private: // data members
    std::map<key,T> m_globals;
};

/// Class which has access to global objects. This class uses raw pointers
/// and using after owner is destructed causes undefined behaviour.
template<class T, class key = std::string>
class global_client : public global<T>
{
public:
    global_client(global const& x);
    void set(key const& key, T const& x);

public: // interface required by global<T,key>
    T& instance(key const& key);
    T const& instance(key const& key) const;

protected: // interface required by global<T,key>
    size_t count() const;
    std::pair<key const&, T*> at(size_t index) const;

private: // data members
    std::map<key,T*> m_globals;
};

} // namespace pt

#include "global.inl"

#endif
