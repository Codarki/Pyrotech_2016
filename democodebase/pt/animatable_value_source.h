#pragma once
#ifndef PT_ANIMATABLE_VALUE_SOURCE_H
#define PT_ANIMATABLE_VALUE_SOURCE_H

#include "string_wrapper_fwd.h"

namespace pt {

/// Abstract base class for animatable value sources of type T.
template<typename T>
class animatable_value_source
{
public:
    virtual bool is_constant() const = 0;
    virtual T value_at(float time) const = 0;
    virtual string_wrapper type_name() const = 0;

    virtual ~animatable_value_source();
};

} // namespace pt

#include "animatable_value_source.inl"

#endif
