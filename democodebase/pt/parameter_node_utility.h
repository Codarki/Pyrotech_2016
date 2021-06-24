#ifndef PT_PARAMETER_NODE_UTILITY_H
#define PT_PARAMETER_NODE_UTILITY_H

#include "parameter_node.h"
#include "animatable_value.h"

// inl
#include "pt/std/cassert.h"

namespace pt {

template<typename T>
std::shared_ptr<animatable_value<T> > convert(
    std::shared_ptr<animatable_value_base> value);
template<typename T>
std::shared_ptr<animatable_value<T> const> convert(
    std::shared_ptr<animatable_value_base const> value);

template<typename T>
T value_at(float time, parameter_node const& node);
template<typename T>
T value_at(float time, parameter_collection const& parameters,
    std::string const& name);

/// Returns animatable value of type T from \a node.
template<typename T>
std::shared_ptr<animatable_value<T> const> get_animatable(
    parameter_node const& node);

/// Returns animatable value of type T from child of \a node.
template<typename T>
std::shared_ptr<animatable_value<T> const> animatable_child(
    string_wrapper const& name,
    parameter_node const& parent_node);

template<>
std::shared_ptr<animatable_value<float> const> animatable_child<float>(
    string_wrapper const& name,
    parameter_node const& parent_node);

template<typename T>
T constant_value(parameter_node& node);

template<typename T>
T constant_value(parameter_node const& node);

} // namespace pt

#include "parameter_node_utility.inl"

#endif
