#ifndef PT_MATH_CR_DATA_SET_H
#define PT_MATH_CR_DATA_SET_H

#include "data_set.h"
#include "pt/boost/assert.h"

namespace pt { namespace math {

template<typename T>
class cr_data_set
{
public: // type definitions
    class point
    {
    public:
        point();
        point(float time, T value, T tangent)
        :   m_time(time)
        ,   m_value(value)
        ,   m_tangent(tangent)
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
            return m_tangent;
        }
        T ending_tangent() const
        {
            return m_tangent;
        }
    private: // data members
        float m_time;
        T m_value;
        T m_tangent;
    };
private:
    typedef std::vector<point> container;
    typedef typename container::iterator iterator;
public:
    typedef typename container::const_iterator const_iterator;
    typedef std::pair<const_iterator,const_iterator> point_pair;
    typedef T result_type;

public:
    cr_data_set()
    {
    }
    cr_data_set(data_set<T> const& values);

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

//private: // helper functions
//    void add(point const& point);

private: // data members
    std::vector<point> m_points;
};

///////

template<typename T>
cr_data_set<T>::cr_data_set(data_set<T> const& values)
{
    assert(values.size() > 1);
    if (values.size() < 2)
        throw pt::any_error("Must have at least two keys in data set");

    data_set<T>::const_iterator prev = values.begin();
    data_set<T>::const_iterator cur = prev;
    data_set<T>::const_iterator next = cur + 1;

    {
        // Special case for first point.
        //data_set<T>::point cur_tangent(
        //    cur->time, (next->value - cur->value) / static_cast<T>(2));
        //m_tangents.add(cur_tangent);

        T tangent = (next->value() - cur->value()) / static_cast<T>(2);
        point p(cur->time(), cur->value(), tangent);
        m_points.push_back(p);
    }
    ++cur;
    ++next;

    while(next != values.end())
    {
        //data_set<T>::point tangent(
        //    cur->time, (next->value - prev->value) / static_cast<T>(2));
        //m_tangents.add(tangent);

        T tangent = (next->value() - prev->value()) / static_cast<T>(2);
        point p(cur->time(), cur->value(), tangent);

        m_points.push_back(p);

        ++prev;
        ++cur;
        ++next;
    }

    {
        // Special case for last point.
        //data_set<T>::point cur_tangent(
        //    cur->time, (cur->value - prev->value) / static_cast<T>(2));
        //m_tangents.add(cur_tangent);

        T tangent = (cur->value() - prev->value()) / static_cast<T>(2);
        point p(cur->time(), cur->value(), tangent);
        m_points.push_back(p);
    }

    //assert(m_positions.size() == m_tangents.size());
}

template<typename T>
inline typename cr_data_set<T>::const_iterator
    cr_data_set<T>::optional_endpoint(float time) const
{
    assert(!m_points.empty());
    return get_optional_endpoint(time, m_points);
}

template<typename T>
inline typename cr_data_set<T>::point_pair cr_data_set<T>::points_at(
    float time) const
{
    assert(!m_points.empty());
    return get_interval(time, *this);
}

}} // namespace pt::math

#endif
