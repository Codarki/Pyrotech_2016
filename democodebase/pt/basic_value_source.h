#ifndef PT_BASIC_VALUE_SOURCE_H
#define PT_BASIC_VALUE_SOURCE_H

#include "animatable_value_source.h"
#include "pt/string_wrapper.h"

namespace pt {

/// User settable source value.
template<typename T>
class basic_value_source : public animatable_value_source<T>
{
public:
    basic_value_source(T value)
    :   m_value(value)
    {
    }
    void set(T value)
    {
        m_value = value;
    }
    /// Implicit cast to T. This class can directly be used as T.
    /// \internal MSVC goes nuts if you try to have both const and non-const.
    operator T&()
    {
        return m_value;
    }

    T& mutable_reference() const
    {
        // TODO EVIL
        return const_cast<T&>(m_value);
    }

public: // interface required by animatable_value_source<T>
    virtual bool is_constant() const
    {
        return false;
    }

    virtual T value_at(float) const
    {
        return m_value;
    }

    virtual string_wrapper type_name() const
    {
        return "basic " + std::string(typeid(m_value).name()); 
    }

private:
    T m_value;
};

} // namespace pt

#endif
