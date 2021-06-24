#ifndef PT_MATH_QUATERNION_DATA_SET_H
#define PT_MATH_KB_DATA_SET_H

#include "data_set.h"
#include "pt/boost/assert.h"

namespace pt { namespace math {

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

template<typename T>
class kb_data_set
{
public: // type definitions
    class point
    {
    public:
        point();
        point(float time, T value)
        :   m_time(time)
        ,   m_value(value)
        ,   m_parameters(0, 0, 0)
        {
        }
        point(float time, T value,
            kochanek_bartels_parameters const& parameters,
            T starting_tangent, T ending_tangent)
        :   m_time(time)
        ,   m_value(value)
        ,   m_parameters(parameters)
        ,   m_starting_tangent(starting_tangent)
        ,   m_ending_tangent(ending_tangent)
        {
        }
        float time() const
        {
            return m_time;
        }
        T value() const
        {
            return m_value;
        }
        T starting_tangent() const
        {
            return m_starting_tangent;
        }
        T ending_tangent() const
        {
            return m_ending_tangent;
        }
    private: // data members
        float m_time;
        kochanek_bartels_parameters m_parameters;
        T m_value;
        T m_starting_tangent;
        T m_ending_tangent;
    };
private:
    typedef std::vector<point> container;
    typedef typename container::iterator iterator;
public:
    typedef typename container::const_iterator const_iterator;
    typedef std::pair<const_iterator,const_iterator> point_pair;
    typedef T result_type;

public:
    kb_data_set();
    kb_data_set(data_set<T> const& values,
        std::vector<kochanek_bartels_parameters> const& parameters);

    const_iterator optional_endpoint(float time) const;
    point_pair points_at(float time) const;

    const_iterator begin() const
    {
        return m_points.begin();
    }
    const_iterator end() const
    {
        return m_points.end();
    }
    size_t size() const
    {
        return m_points.size();
    }

private: // helper functions
    void add(point const& point);

private: // data members
    std::vector<point> m_points;
};

// .inl
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
inline kb_data_set<T>::point::point()
:   m_parameters(0, 0, 0)
,   m_time(0)
,   m_value(static_cast<T>(0))
,   m_starting_tangent(static_cast<T>(0))
,   m_ending_tangent(static_cast<T>(0))
{
}

template<typename T>
inline typename kb_data_set<T>::point create_point(
    typename data_set<T>::const_iterator cur, T prev, T next,
    typename std::vector<kochanek_bartels_parameters>::const_iterator param_iter)
{
    T starting_tangent
        = starting_tangent_param1(*param_iter) * prev
        + starting_tangent_param2(*param_iter) * next;

    T ending_tangent
        = ending_tangent_param1(*param_iter) * prev
        + ending_tangent_param2(*param_iter) * next;

    return kb_data_set<T>::point(cur->time(), cur->value(), *param_iter,
        starting_tangent, ending_tangent);
}

template<typename T>
inline kb_data_set<T>::kb_data_set()
{
}

template<typename T>
inline kb_data_set<T>::kb_data_set(data_set<T> const& values,
    std::vector<kochanek_bartels_parameters> const& parameters)
{
    assert(values.size() == parameters.size());
    assert(values.size() > 0);

    if(values.size() == 1)
    {
        data_set<T>::const_iterator cur = values.begin();
        point p(cur->time(), cur->value());
        add(p);

        return;
    }

    data_set<T>::const_iterator prev = values.begin();
    data_set<T>::const_iterator cur = values.begin();
    data_set<T>::const_iterator next = cur + 1;

    std::vector<kochanek_bartels_parameters>::const_iterator param_iter
        = parameters.begin();

    {
        // Special case for first point.
        T tmp_prev = next->value() - cur->value();
        T tmp_next = next->value() - cur->value();

        point p = create_point(cur, tmp_prev, tmp_next, param_iter);
        add(p);
    }
    ++cur;
    ++next;
    ++param_iter;

    while(next != values.end())
    {
        T tmp_prev = cur->value() - prev->value();
        T tmp_next = next->value() - cur->value();

        point p = create_point(cur, tmp_prev, tmp_next, param_iter);
        add(p);

        ++prev;
        ++cur;
        ++next;
        ++param_iter;
    }

    {
        // Special case for last point.
        T tmp_prev = cur->value() - prev->value();
        T tmp_next = cur->value() - prev->value();

        point p = create_point(cur, tmp_prev, tmp_next, param_iter);
        add(p);
    }
}

template<typename T>
inline typename kb_data_set<T>::const_iterator
    kb_data_set<T>::optional_endpoint(float time) const
{
    assert(!m_points.empty());
    return get_optional_endpoint(time, m_points);
}

template<typename T>
inline typename kb_data_set<T>::point_pair kb_data_set<T>::points_at(
    float time) const
{
    assert(!m_points.empty());
    return get_interval(time, *this);
}

template<typename T>
inline void kb_data_set<T>::add(point const& point)
{
    iterator i = m_points.begin();
    while(i != m_points.end() && i->time() < point.time())
        ++i;

    // no keys with same time.
    //assert(i == m_points.end() || i->time != point.time);
    if(i != m_points.end() && i->time() == point.time())
    {
        log("Warning: Ignoring spline key with equal time.");
        return;
    }

    if(i != m_points.end())
        ++i;

    m_points.insert(i, point);
}

}} // namespace pt::math

#endif
