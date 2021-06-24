#ifndef PT_MATH_ROUNDING_H
#define PT_MATH_ROUNDING_H

namespace pt { namespace math {

inline int floor(float value)
{
    return static_cast<int>(value);
}

inline int floor(double value)
{
    return static_cast<int>(value);
}

inline int round(float value)
{
    return floor(value + 0.5f);
}

inline int round(double value)
{
    return floor(value + 0.5);
}

}} // namespace pt::math

#endif
