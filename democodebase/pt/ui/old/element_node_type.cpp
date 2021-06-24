#include "element_node_type.h"

namespace pt { namespace ui {

std::string const& element_node_type::name() const
{
    static std::string arr("element_node");
    return arr;
}

shared_ptr<operator_graph::node> element_node_type::create_instance() const
{
    return shared_ptr<operator_graph::node>();
}

}} // namespace pt::ui
