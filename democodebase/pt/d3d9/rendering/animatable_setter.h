#ifndef PT_D3D9_RENDERING_ANIMATABLE_SETTER_H
#define PT_D3D9_RENDERING_ANIMATABLE_SETTER_H

#include "constant_setter.h"
#include "pt/animatable_value.h"

namespace pt { namespace d3d9 { namespace rendering {

/// Constant setter from animatable_value of type T.
template<typename T>
class animatable_setter : public constant_setter
{
public:
    animatable_setter(std::shared_ptr<animatable_value<T> > value);

public: // interface required by variable_setter
    void update_target(float time) const;

private: // data members
    std::shared_ptr<animatable_value<T> > m_value;
};

//
template<typename T>
class animatable_array_setter : public constant_setter
{
public:
    animatable_array_setter(std::vector<animatable_value<T>> values)
        : m_values(values)
    {
        m_values_at_time.resize(m_values.size());
    }

public: // interface required by variable_setter
    void update_target(float time) const
    {
        for (size_t i = 0; i < m_values.size(); ++i)
        {
            m_values_at_time[i] = m_values.value_at(time);
        }
        set_constant(constant(), constants(), m_values_at_time, C);
    }

private: // data members
    std::vector<animatable_value<T>> m_values;
    std::vector<T> m_values_at_time;
};

template<typename T>
class basic_array_setter : public constant_setter
{
public:
    basic_array_setter(std::vector<T> values)
        : m_values(values)
    {
    }

public: // interface required by variable_setter
    void update_target(float) const
    {
        set_constant(constant(), constants(), m_values);
    }

private: // data members
    std::vector<T> m_values;
};


}}} // namespace pt::d3d9::rendering

#include "animatable_setter.inl"

#endif
