#ifndef PT_UI_ELEMENT_NODE_H
#define PT_UI_ELEMENT_NODE_H

#include "pt/operator_graph/node.h"

namespace pt { namespace ui {

class element_node : public operator_graph::node
{
public:
    element_node();

private: // interface required by operator_graph::node
    void parse(config_node const& config);
};

}} // namespace pt::ui

#endif
