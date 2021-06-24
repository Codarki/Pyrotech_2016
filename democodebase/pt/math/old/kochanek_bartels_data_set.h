#ifndef PT_MATH_KOCHANEK_BARTELS_DATA_SET_H
#define PT_MATH_KOCHANEK_BARTELS_DATA_SET_H

#include "data_set.h"
#include "../vector.h"
#include "pt/boost/assert.h"

namespace pt { namespace math {

/// Setting each parameter to zero would give a Catmull-Rom spline.
/// The tension parameter changes the length of the tangent vector. The bias
/// parameter primarily changes the direction of the tangent vector. The
/// continuity parameter changes the sharpness in change between tangents.
struct kochanek_bartels_parameters
{
    kochanek_bartels_parameters(float tension_, float bias_, float continuity_)
    :   tension(tension_)
    ,   bias(bias_)
    ,   continuity(continuity_)
    {
    }
    float tension;
    float bias;
    float continuity;
};

/// Kochanek-Bartels spline is a cubic Hermite spline with tension, bias, and
/// continuity parameters defined to change the behavior of the tangents.
template<typename T>
class kochanek_bartels_data_set
{
public:
    typedef T result_type;

public:
    void set_positions(data_set<T> const& positions);
    void set_parameters(
        std::vector<kochanek_bartels_parameters> const& parameters);
    void calculate_tangents();

    bool in_range(float time, T& value) const;
    data_set<float>::point_pair points_at(float time) const;
    data_set<float>::point_pair tangents_at(float time) const;

    data_set<float>::const_iterator point_end() const
    {
        return m_positions.end();
    }
    size_t position_count() const
    {
        return m_positions.size();
    }

private: // data members
    std::vector<kochanek_bartels_parameters> m_parameters;
    data_set<T> m_positions;
    data_set<T> m_starting_tangents;
    data_set<T> m_ending_tangents;
};

// .inl
template<typename T>
inline void kochanek_bartels_data_set<T>::set_positions(
    data_set<T> const& positions)
{
    assert(positions.size() > 0);
    m_positions = positions;
}

template<typename T>
inline void kochanek_bartels_data_set<T>::set_parameters(
    std::vector<kochanek_bartels_parameters> const& parameters)
{
    m_parameters = parameters;
}

inline float starting_tangent_param1(kochanek_bartels_parameters const& p)
{
    return ((1.0f - p.tension) * (1.0f + p.bias) * (1.0f - p.continuity))
        / 2.0f;
}

inline float starting_tangent_param2(kochanek_bartels_parameters const& p)
{
    return ((1.0f - p.tension) * (1.0f - p.bias) * (1.0f + p.continuity))
        / 2.0f;
}

inline float ending_tangent_param1(kochanek_bartels_parameters const& p)
{
    return ((1.0f - p.tension) * (1.0f + p.bias) * (1.0f + p.continuity))
        / 2.0f;
}

inline float ending_tangent_param2(kochanek_bartels_parameters const& p)
{
    return ((1.0f - p.tension) * (1.0f - p.bias) * (1.0f - p.continuity))
        / 2.0f;
}

template<typename T>
inline void kochanek_bartels_data_set<T>::calculate_tangents()
{
    assert(m_positions.size() > 0);
    if(m_positions.size() == 1)
        return;

    assert(m_positions.size() == m_parameters.size());
    assert(m_starting_tangents.size() == 0);
    assert(m_ending_tangents.size() == 0);

    data_set<T>::const_iterator prev = m_positions.begin();
    data_set<T>::const_iterator cur = m_positions.begin();
    data_set<T>::const_iterator next = cur + 1;

    std::vector<kochanek_bartels_parameters>::const_iterator param_iter
        = m_parameters.begin();

    {
        // Special case for first point.
        T tmp_prev = next->value - cur->value;
        T tmp_next = next->value - cur->value;

        T starting_tangent
            = starting_tangent_param1(*param_iter) * tmp_prev
            + starting_tangent_param2(*param_iter) * tmp_next;

        T ending_tangent
            = ending_tangent_param1(*param_iter) * tmp_prev
            + ending_tangent_param2(*param_iter) * tmp_next;

        m_starting_tangents.add(cur->time, starting_tangent);
        m_ending_tangents.add(cur->time, ending_tangent);
    }
    ++cur;
    ++next;
    ++param_iter;

    while(next != m_positions.end())
    {
        T tmp_prev = cur->value - prev->value;
        T tmp_next = next->value - cur->value;

        T starting_tangent
            = starting_tangent_param1(*param_iter) * tmp_prev
            + starting_tangent_param2(*param_iter) * tmp_next;

        T ending_tangent
            = ending_tangent_param1(*param_iter) * tmp_prev
            + ending_tangent_param2(*param_iter) * tmp_next;

        m_starting_tangents.add(cur->time, starting_tangent);
        m_ending_tangents.add(cur->time, ending_tangent);

        ++prev;
        ++cur;
        ++next;
        ++param_iter;
    }

    {
        // Special case for last point.
        T tmp_prev = cur->value - prev->value;
        T tmp_next = cur->value - prev->value;

        T starting_tangent
            = starting_tangent_param1(*param_iter) * tmp_prev
            + starting_tangent_param2(*param_iter) * tmp_next;

        T ending_tangent
            = ending_tangent_param1(*param_iter) * tmp_prev
            + ending_tangent_param2(*param_iter) * tmp_next;

        m_starting_tangents.add(cur->time, starting_tangent);
        m_ending_tangents.add(cur->time, ending_tangent);
    }

    assert(m_positions.size() == m_starting_tangents.size());
    assert(m_positions.size() == m_ending_tangents.size());
}

template<typename T>
inline bool kochanek_bartels_data_set<T>::in_range(
    float time, T& value) const
{
    return !get_optional_endpoint(time, m_positions, value);
}

template<typename T>
inline data_set<float>::point_pair kochanek_bartels_data_set<T>::points_at(
    float time) const
{
    return get_interval(time, m_positions);
}

template<typename T>
inline data_set<float>::point_pair kochanek_bartels_data_set<T>::tangents_at(
    float time) const
{
    data_set<float>::point_pair starting
        = get_interval(time, m_starting_tangents);
    data_set<float>::point_pair ending
        = get_interval(time, m_ending_tangents);

    return data_set<float>::point_pair(starting.first, ending.second);
}

}} // namespace pt::math

#endif
