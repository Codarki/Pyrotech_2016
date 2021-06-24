#ifndef PT_UI_ELEMENT_NODE_TYPE_H
#define PT_UI_ELEMENT_NODE_TYPE_H

#include "pt/operator_graph/node_type.h"

namespace pt { namespace ui {

class element_node_type
{
public: // interface required by operator_graph::node_type
    std::string const& name() const;
    shared_ptr<operator_graph::node> create_instance() const;
};

}} // namespace pt::ui

#endif
