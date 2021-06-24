#ifndef PT_UI_EXPRESSIONS_TEXTURE_NODE_H
#define PT_UI_EXPRESSIONS_TEXTURE_NODE_H

#include "pt/operations/nodes/expression_node_type.h"
#include "texture_expression.h"
#include "texture_procedure.h"

namespace pt { namespace operations {
    class node_type_registration_manager;
}} // namespace pt::operations

namespace pt { namespace ui { namespace expressions {

typedef operations::nodes::expression_node_type<texture_expression>
    texture_node_type;

typedef operations::nodes::expression_node_type<texture_procedure_expression>
    texture_procedure_node_type;

void register_ui_node_types_texture(
    operations::node_type_registration_manager& manager);

}}} // namespace pt::ui::expressions

#endif
