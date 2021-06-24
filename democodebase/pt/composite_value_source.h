#pragma once
#ifndef PT_COMPOSITE_VALUE_SOURCE_H
#define PT_COMPOSITE_VALUE_SOURCE_H

#include "pt/animatable_value_source.h"
#include "pt/string_wrapper.h"
#include "pt/string_builder.h"

namespace pt {

/// Composite of two value sources of type T. Immutable object.
template<typename T>
class composite_value_source : public animatable_value_source<T>
{
public:
    composite_value_source(
        std::shared_ptr<animatable_value_source<T>> curve0,
        std::shared_ptr<animatable_value_source<T>> curve1)
    : m_curve0(curve0)
    , m_curve1(curve1)
    {
        assert(m_curve0);
        assert(m_curve1);
    }

    // interface required by animatable_value_source<T>
    bool is_constant() const
    {
        return m_curve0->is_constant() && m_curve1->is_constant();
    }

    T value_at(float time) const
    {
        T result = m_curve0->value_at(time) + m_curve1->value_at(time);
        return result;
    }

    string_wrapper type_name() const
    {
        return string_wrapper("composite_vector3f_value_source");
    }

private: // data members
    std::shared_ptr<animatable_value_source<T>> m_curve0;
    std::shared_ptr<animatable_value_source<T>> m_curve1;
};

} // namespace pt

#endif
