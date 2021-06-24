#ifndef PT_NONE_H
#define PT_NONE_H

#include "inline.h"
#include <limits>

namespace pt {

/// Returns a value that can be interpreted as no value at all. Only
/// spezialiations exists.
template<typename T>
T none();

template<>
PT_INLINE size_t none<size_t>()
{
	return ~static_cast<size_t>(0);
}

template<>
PT_INLINE unsigned long none<unsigned long>()
{
	return ~static_cast<unsigned long>(0);
}

template<>
PT_INLINE int none<int>()
{
    return std::numeric_limits<int>::min();
}

/// Returns \c true x is none<T>().
template<typename T>
bool is_none(T x)
{
    return x == none<T>();
}

} // namespace pt

#endif
