#include "texture_node.h"
#include "pt/operations/node_type_registration_manager.h"
#include "pt/operations/nodes/null_input.h"
#include "pt/expressions/expression_signals.h"

namespace pt { namespace ui { namespace expressions {

void register_ui_node_types_texture(
    operations::node_type_registration_manager& manager)
{
    using namespace operations;

    manager.add(shared_ptr<nodes::type>(new texture_node_type(
        "ui.texture", nodes::create_null_inputs<1>())));

    manager.add(shared_ptr<nodes::type>(new texture_procedure_node_type(
        "ui.texture_proc", nodes::create_null_inputs<1>())));
}

}}} // namespace pt::ui::expressions
