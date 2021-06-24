#pragma once
#ifndef PT_CONSTANT_VALUE_SOURCE_H
#define PT_CONSTANT_VALUE_SOURCE_H

#include "pt/animatable_value_source.h"
#include "pt/string_wrapper.h"
#include "pt/string_builder.h"

namespace pt {

/// Constant value of type T. Immutable object.
template<typename T>
class constant_value_source final : public animatable_value_source<T>
{
public:
    constant_value_source(T value);

    operator T const&();

public: // interface required by animatable_value_source<T>
    virtual bool is_constant() const final override;
    virtual T value_at(float) const final override;
    virtual string_wrapper type_name() const final override;

private:
    T m_value;
};

} // namespace pt

#include "constant_value_source.inl"

#endif
