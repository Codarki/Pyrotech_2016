#ifndef PT_PARAMETER_NODE_UTILITY_INL
#define PT_PARAMETER_NODE_UTILITY_INL

namespace pt {

template<typename T>
inline std::shared_ptr<animatable_value<T> > convert(
    std::shared_ptr<animatable_value_base> value)
{
    std::shared_ptr<animatable_value<T> > anim_value
        = std::dynamic_pointer_cast<animatable_value<T> >(value);
    assert(anim_value);
    return anim_value;
}

template<typename T>
inline std::shared_ptr<animatable_value<T> const> convert(
    std::shared_ptr<animatable_value_base const> value)
{
    std::shared_ptr<animatable_value<T> const> anim_value
        = std::dynamic_pointer_cast<animatable_value<T> const>(value);
    assert(anim_value);
    return anim_value;
}

template<typename T>
inline std::shared_ptr<animatable_value<T> const> get_animatable(
    parameter_node const& node)
{
    return convert<T>(node.value());
}

template<typename T>
std::shared_ptr<animatable_value<T> const> animatable_child(
    string_wrapper const& name,
    parameter_node const& parent_node)
{
    return get_animatable<T>(*get_child(name, parent_node));
}

template<typename T>
inline T value_at(float time, parameter_node const& node)
{
    return convert<T>(node.value())->value_at(time);
}

template<typename T>
inline T value_at(float time, parameter_collection const& parameters,
    std::string const& name)
{
    parameter_collection::const_iterator i = parameters.child(name);
    assert(i != parameters.end());
    return value_at<T>(time, **i);
}

template<typename T>
T constant_value(parameter_node& node)
{
    assert(node.value()->is_constant());
    return convert<T>(node.value())->value_at(0.0f);
}

template<typename T>
T constant_value(parameter_node const& node)
{
    assert(node.value()->is_constant());
    return convert<T>(node.value())->value_at(0.0f);
}

} // namespace pt

#endif
