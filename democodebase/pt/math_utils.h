#pragma once
#ifndef PT_MATH_UTILS_H
#define PT_MATH_UTILS_H

#include <cmath>

namespace pt {

// testing

/// Clamps the \a value between \a min and \a max. \return A clamped
/// \a value between \a min and \a max.
template<typename T>
T clamp(T const& value, T const& min, T const& max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	return value;
}

/// Linear interpolation. \return A value by linear interpolation between
/// \a value_1 and \a value_2.
template<typename T>
T lerp(T const& value_1, T const& value_2, float t)
{
	float const clamped_t = clamp(t, 0.0f, 1.0f);
	return (1.0f - clamped_t) * value_1 + clamped_t * value_2;
}

/// \return Average of \a a and \a b.
// there are few overloads in vector class
template<typename T>
T average(T const& a, T const& b)
{
	return (a + b) / static_cast<T>(2);
}
/// \return Average of \a a and \a b.
inline float average(float a, float b)
{
	return (a + b) * 0.5f;
}

inline float parameterize(float a_min, float a_max, float b_min, float b_max, float value)
{
    //float a_min; // 10.0f
    //float a_max; // 110.0f
    //float b_min; // 1.0f
    //float b_max; // 2.0f
    //float v = 20.0f; //

    const float range1 = a_max - a_min; // 100
    const float range2 = b_max - b_min; // 1

    const float tmp = (value - a_min) / range1; // 0 -- 1
    return tmp * range2 + b_min;
}

/// Reciprocal of the square root approximation depending on IEEE
/// representation. Relative error is less than 4%.
/// \return Approximation of 1.0f / sqrt(x).
inline float fast_reciprocal_sqrt(float x)
{
    // Written by Greg Walsh, and implemented into Quake 3 by Gary Tarolli.
    float xhalf = 0.5f * x;
    int i = *(reinterpret_cast<int*>(&x));
    i = 0x5f3759df - (i >> 1);
    x = *(reinterpret_cast<float*>(&i));
    x = x * (1.5f - xhalf * x * x);
    return x;
}

/// \return \c true if \a value is so close to \c 0, that it can be thought as
/// being result of rounding errors.
inline bool within_denormal(float const& value)
{
    // epsilon 1 * 10^(-6)
    return std::abs(value) < 1.0E-6;
}

/// \return \c true if \a value is so close to \c 0, that it can be thought as
/// being result of rounding errors.
inline bool within_denormal(double const& value)
{
    // epsilon 1 * 10^(-13)
    return std::abs(value) < 1.0E-13;
}

/// \return Denormalized \a value at -1, 0 or 1.
template<typename T>
T denormalize(T const& value)
{
    if (within_denormal(value))
        return static_cast<T>(0);

    if (within_denormal(value - static_cast<T>(1)))
        return static_cast<T>(1);

    if (within_denormal(value + static_cast<T>(1)))
        return static_cast<T>(-1);

    return value;
}

inline float to_radians(float degree)
{
    return degree / 180.0f * 3.1415f;
}

inline double to_radians(double degree)
{
    return degree / 180.0 * 3.1415;
}

} // namespace pt

#endif
