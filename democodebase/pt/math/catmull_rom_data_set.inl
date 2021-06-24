#ifndef PT_MATH_CATMULL_ROM_DATA_SET_INL
#define PT_MATH_CATMULL_ROM_DATA_SET_INL

namespace pt { namespace math {

// catmull_rom_data_set

template<typename T>
inline catmull_rom_data_set<T>::catmull_rom_data_set()
{
}

template<typename T>
inline void catmull_rom_data_set<T>::set_positions(
    data_set<T> const& positions)
{
    m_positions = positions;
    //calculate_tangents();
}

template<typename T>
inline void catmull_rom_data_set<T>::calculate_tangents()
{
    assert(m_positions.size() > 1);
    assert(m_tangents.size() == 0);

    data_set<T>::const_iterator prev = m_positions.begin();
    data_set<T>::const_iterator cur = prev;
    data_set<T>::const_iterator next = cur + 1;

    {
        // Special case for first point.
        data_set<T>::point cur_tangent(
            cur->time, (next->value - cur->value) / static_cast<T>(2));
        m_tangents.add(cur_tangent);
    }
    ++cur;
    ++next;

    while(next != m_positions.end())
    {
        data_set<T>::point tangent(
            cur->time, (next->value - prev->value) / static_cast<T>(2));
        m_tangents.add(tangent);

        ++prev;
        ++cur;
        ++next;
    }

    {
        // Special case for last point.
        data_set<T>::point cur_tangent(
            cur->time, (cur->value - prev->value) / static_cast<T>(2));
        m_tangents.add(cur_tangent);
    }

    assert(m_positions.size() == m_tangents.size());
}

template<typename T>
inline bool catmull_rom_data_set<T>::in_range(float time, T& value) const
{
    return !get_optional_endpoint(time, m_positions, value);
}

template<typename T>
inline data_set<float>::point_pair catmull_rom_data_set<T>::points_at(
    float time) const
{
    return get_interval(time, m_positions);
}

template<typename T>
inline data_set<float>::point_pair catmull_rom_data_set<T>::tangents_at(
    float time) const
{
    return get_interval(time, m_tangents);
}

}} // namespace pt::math

#endif
