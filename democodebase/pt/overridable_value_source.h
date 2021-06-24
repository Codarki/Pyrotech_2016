#ifndef PT_OVERRIDABLE_VALUE_SOURCE_H
#define PT_OVERRIDABLE_VALUE_SOURCE_H

#include "basic_value_source.h"
#include "animatable_value.h"
#include "math_utils.h"

namespace pt {

/// Additional indirection with support of user settable value source which
/// will override given value.
template<typename T>
class overridable_value_source : public animatable_value_source<T>
{
public:
    overridable_value_source(std::shared_ptr<animatable_value<T> const> value)
    :   m_value(value)
    {
        assert(m_value);
    }
    void set_value_source(std::shared_ptr<basic_value_source<T> > override_value)
    {
        m_override_value = override_value;
    }
    void set(T const& value)
    {
        if (!m_override_value)
            m_override_value.reset(new basic_value_source<T>(value));
        else
            m_override_value->set(value);
    }

public: // interface required by animatable_value_source<T>
    virtual bool is_constant() const
    {
        return false;
    }

    virtual T value_at(float time) const
    {
        if (m_override_value)
            return m_override_value->value_at(time);

        return m_value->value_at(time);
    }

    virtual string_wrapper type_name() const
    {
        if (m_override_value)
            return m_override_value->type_name();

        return m_value->type_name();
    }

private: // data members
    std::shared_ptr<animatable_value<T> const> m_value;
    std::shared_ptr<basic_value_source<T> > m_override_value;
};

template<typename T>
void operator+=(std::shared_ptr<overridable_value_source<T> >& source, T scalar)
{
}

inline void add_5_percent(overridable_value_source<float>& source, float time)
{
    float current = source.value_at(time);
    current += current * 0.05f;
    source.set(current);
}

inline void dec_5_percent(overridable_value_source<float>& source, float time)
{
    float current = source.value_at(time);
    current -= current * 0.05f;
    source.set(current > 0.0f ? current : 0.0f);
}

inline void clamp_01(overridable_value_source<float>& source, float time)
{
    float current = source.value_at(time);
    source.set(pt::clamp(current, 0.0f, 1.0f));
}

} // namespace pt

#endif
