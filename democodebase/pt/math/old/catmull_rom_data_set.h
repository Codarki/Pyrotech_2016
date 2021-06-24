#ifndef PT_MATH_CATMULL_ROM_DATA_SET_H
#define PT_MATH_CATMULL_ROM_DATA_SET_H

#include "data_set.h"
#include "pt/boost/assert.h"
#include "pt/debug_only.h"

namespace pt { namespace math {

/// Catmull–Rom spline is Cardinal spline with 0 tension.
/// tangent = (Pk(+1) - Pk(-1)) / 2
template<typename T>
class catmull_rom_data_set
{
public:
    typedef T result_type;

public:
    catmull_rom_data_set();
    void set_positions(data_set<T> const& positions);
    void calculate_tangents();

    bool in_range(float time, T& value) const;
    data_set<float>::point_pair points_at(float time) const;
    data_set<float>::point_pair tangents_at(float time) const;

    data_set<float>::const_iterator point_end() const
    {
        return m_positions.end();
    }

private: // data members
    data_set<T> m_positions;
    data_set<T> m_tangents;
};

// Free functions.

/// Returns pair of control points for interval enclosing \a time. If \a time
/// is a exact match for a control point, second point will be set to
/// data.end().
template<typename T>
typename data_set<T>::point_pair get_interval(float time,
    data_set<T> const& data);

/// Returns \c false if time is in range of \a data. Else returns \c true and
/// sets \a value as first or last control point value.
template<typename T>
bool get_optional_endpoint(float time, data_set<T> const& data, T& value);

}} // namespace pt::math

#include "catmull_rom_data_set.inl"

#endif
