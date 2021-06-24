#pragma once
#ifndef PT_ANIMATABLE_VALUE_BASE_H
#define PT_ANIMATABLE_VALUE_BASE_H

#include "string_wrapper_fwd.h"

namespace pt {

/// Abstract base class for all animatable values. There is no way finding
/// out the real type of the value, except by using dynamic_cast.
class animatable_value_base
{
public:
    virtual ~animatable_value_base();

public: // interface which subclasses must implement
    /// Returns \c true if this value is constant.
    virtual bool is_constant() const = 0;

    virtual string_wrapper type_name() const = 0;
};

} // namespace pt

#endif
