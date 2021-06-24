#ifndef PT_ANIMATABLE_VALUE_UTILITY_H
#define PT_ANIMATABLE_VALUE_UTILITY_H

#include "animatable_value.h"
#include "constant_value_source.h"
#include "pt/basic_types.h"
#include "pt/vector.h"
#include "pt/std/string.h"

namespace pt {

// Utility functions.

std::shared_ptr<animatable_value<float> > create_constant_value(float value);
std::shared_ptr<animatable_value<int> > create_constant_value(int value);
std::shared_ptr<animatable_value<bool> > create_constant_value(bool value);
std::shared_ptr<animatable_value<std::string> > create_constant_value(
    std::string const& value);
std::shared_ptr<animatable_value<std::string> > create_constant_value(
    char const* value);

std::shared_ptr<animatable_value<uint32> > create_constant_value(uint32 value);

/// Returns constant vector of type T.
template<typename T, int dim>
inline std::shared_ptr<animatable_value<vector<T,dim> > > create_constant_value(
    vector<T,dim> const& value)
{
    std::shared_ptr<constant_value_source<vector<T,dim> > > source(
        new constant_value_source<vector<T,dim> >(value));

    return std::shared_ptr<animatable_value<vector<T,dim> > >(
        new animatable_value<vector<T,dim> >(source));
}

} // namespace pt

#endif
