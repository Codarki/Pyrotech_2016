#ifndef PT_P3D_FORMAT_ATTRIBUTE_MAP_H
#define PT_P3D_FORMAT_ATTRIBUTE_MAP_H

#include "pt/boost/assert.h"
//#include "pt/std/map.h"
#include "pt/std/vector.h"

namespace p3d_format {

#if 0
/// Container of attributes of type Value mapped to keys of type T.
template<typename T, typename Value>
class attribute_map
{
private: // type definitions
    typedef std::map<T,Value> container;
public:
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

public:
    void add(T const& key, Value const& attribute);
    void add(T const& key, Value&& attribute);

    Value& value_for(T const& key);
    Value const& value_for(T const& key) const;

    const_iterator find(T const& key) const;

    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

    bool empty() const;

private: // data members
    container m_attributes;
};

#else
// implemented with vector

/// Container of attributes of type Value mapped to keys of type T.
template<typename T, typename Value>
class attribute_map
{
private: // type definitions
    struct entry
    {
        T key;
        Value value;

        entry(entry&& other)
        : key(static_cast<T&&>(other.key))
        , value(static_cast<Value&&>(other.value))
        {
        }

        entry(T const& key_)
        : key(key_)
        , value()
        {
        }

        entry(T const& key_, Value const& value_)
        : key(key_)
        , value(value_)
        {
        }

        entry(T const& key_, Value&& value_)
        : key(key_)
        , value(std::move(value_))
        {
        }

		entry& operator=(entry const& rhs) = delete;
		//entry& operator=(entry const& rhs) const
		//{
		//	key = T(rhs.key);
		//	value = Value(rhs.value);
		//	return *this;
		//}

		entry& operator=(entry&& rhs)
		{
			using std::move;
			key = move(rhs.key);
			value = move(rhs.value);
			return *this;
		}

        bool operator<(entry const& rhs) const
        {
            return key < rhs.key;
        }
    };
    typedef std::vector<entry> container;
public:
    typedef typename container::iterator iterator;

public:
    attribute_map()
    : m_is_inserting(false)
    {
    }

    void reserve(size_t count);

    void begin_inserting();
    void end_inserting();

    void add(T const& key, Value const& attribute);
    void add(T const& key, Value&& attribute);

    Value& value_for(T const& key);
    Value const& value_for(T const& key) const;

    bool empty() const;

private: // data members
    container m_attributes;
    bool m_is_inserting;
};

#endif

} // namespace p3d_format

#include "attribute_map.inl"

#endif
