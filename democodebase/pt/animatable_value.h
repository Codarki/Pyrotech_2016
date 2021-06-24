#pragma once
#ifndef PT_ANIMATABLE_VALUE_H
#define PT_ANIMATABLE_VALUE_H

#include "animatable_value_fwd.h"
#include "animatable_value_base.h"
#include "animatable_value_source.h"
#include "pt/string_wrapper.h"
#include "pt/std/memory.h"

// inl
#include "pt/std/cassert.h"

namespace pt {

/// Animatable value of type T. Can be animated or constant.
template<typename T>
class animatable_value : public animatable_value_base
{
public:
    animatable_value();
    animatable_value(std::shared_ptr<animatable_value_source<T> > source);

    animatable_value(animatable_value const& other) = default;
    animatable_value(animatable_value&& other); // = default;
    animatable_value const& operator=(animatable_value const& other); // = default;
    animatable_value const& operator=(animatable_value&& other); // = default;

    T value_at(float time) const;

    //void set_source(std::shared_ptr<animatable_value_source<T> > source);
    std::shared_ptr<animatable_value_source<T> > const& source();
    std::shared_ptr<animatable_value_source<T> > const& source() const;

public: // interface required by animatable_value_base
    virtual bool is_constant() const final override;
    virtual string_wrapper type_name() const final override;

private: // data members
    std::shared_ptr<animatable_value_source<T> > m_source;
};

} // namespace pt

#include "animatable_value.inl"

#endif
