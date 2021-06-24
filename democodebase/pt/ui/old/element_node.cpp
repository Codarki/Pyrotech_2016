#include "element_node.h"
#include "element_expression.h"
#include "value_element.h"
#include "pt/operator_graph/output_pin.h"

namespace pt { namespace ui {

element_node::element_node()
{
    // Set expression output
    set_output_pin_count(1);
    shared_ptr<operator_graph::output_pin> const& pin = output_pin(0);

    expressions::expression_ptr e(new element_expression(
        type_element::instance()));

    pin->set_expression(e);

    // Set expression inputs
    set_input_pin_count(5);
}

void element_node::parse(config_node const&)
{
    //shared_ptr<element_expression> expression(new element_expression);
}

}} // namespace pt::ui
