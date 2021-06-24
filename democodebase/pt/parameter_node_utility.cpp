#include "parameter_node_utility.h"

namespace pt {

template<>
std::shared_ptr<animatable_value<float> const> animatable_child<float>(
    string_wrapper const& name,
    parameter_node const& parent_node)
{
    return get_animatable<float>(*get_child(name, parent_node));
}

} // namespace pt
