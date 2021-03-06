#ifndef PT_MATH_DATA_SET_INL
#define PT_MATH_DATA_SET_INL

namespace pt { namespace math {

template<typename T>
inline data_set<T>::point::point(float time_, T value_)
:   m_time(time_)
,   m_value(value_)
{
}

template<typename T>
typename data_set<T>::iterator data_set<T>::add(float time, T value)
{
    return add(point(time, value));
}

template<typename T>
typename data_set<T>::iterator data_set<T>::add(point const& point)
{
    iterator i = m_points.begin();
    while(i != m_points.end() && i->time() < point.time())
        ++i;

    // no keys with same time.
    //assert(i == m_points.end() || i->time != point.time);
    if (i != m_points.end() && i->time() == point.time())
    {
        log("Warning: Ignoring spline key with equal time.");
        return m_points.end();
    }

    //if (i != m_points.end())
    //    ++i;

    return m_points.insert(i, point);
}

template<typename DataSet>
inline typename DataSet::const_iterator get_optional_endpoint(float time,
    DataSet const& data)
{
    assert(data.size() > 0);
    if (data.size() == 1)
    {
        return data.begin();
    }
    DataSet::const_iterator first = data.begin();
    DataSet::const_iterator last = data.end() - 1;

    if (time <= first->time())
        return first;

    if (time >= last->time())
        return last;

    return data.end();
}

template<typename DataSet>
typename DataSet::point_pair get_interval(float time, DataSet const& data)
{
    assert(get_optional_endpoint(time, data) == data.end());
    assert(data.size() > 1);

    DataSet::const_iterator current = data.begin();
    DataSet::const_iterator next = current + 1;

    assert(current != data.end());

    //if (time < current->time)
    //{
    //    // Time before first key.
    //    current = data.end();
    //    next = data.begin();
    //    return data_set<float>::point_pair(current, next);
    //}

    while(next != data.end())
    {
        if (current->time() == time)
        {
            // Exact match for key.
            next = data.end();
            return DataSet::point_pair(current, next);
        }

        if (current->time() < time && time < next->time())
            return DataSet::point_pair(current, next);

        ++current;
        ++next;
    }

    assert(false);
    return DataSet::point_pair(data.end(), data.end());
    //// Time after last key.
    //current = data.end() - 1;
    //next = data.end() - 1;
    //return data_set<float>::point_pair(current, next);
}

}} // namespace pt::math

#endif
