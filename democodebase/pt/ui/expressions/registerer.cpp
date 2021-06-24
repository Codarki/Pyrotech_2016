#include "registerer.h"
#include "element_registration.h"
#include "function_element_type.h"
#include "window.h"
#include "node_visualizer.h"
#include "elements/image.h"
#include "elements/value_window.h"
#include "elements/value_texture.h"
#include "expressions/texture_node.h"
//#include "pt/expressions/function_registry.h"

namespace pt { namespace ui {
namespace {
    // Create function for ui::window element.
    //shared_ptr<element> create_window()
    //{
    //    return shared_ptr<element>(new window);
    //}
} // anonymous namespace

registerer::registerer(
    shared_ptr<pt::expressions::function_registry> functions,
    shared_ptr<operations::node_type_registry> node_types)
:   m_function_registration_manager(functions)
,   m_node_type_manager(node_types)
{
    // Register pt::ui namespace elements...

    // window element
    //shared_ptr<element_type> type(new function_element_type(
    //    "window", create_window));
    //shared_ptr<element_registration> registration(new element_registration(
    //    type));
    //m_element_registration_manager.add(registration);

    elements::register_values_ui_image(m_function_registration_manager);
    elements::register_values_ui_window(m_function_registration_manager);
    elements::register_values_ui_texture(m_function_registration_manager);

    register_ui_node_types_visualizer(m_node_type_manager);
    expressions::register_ui_node_types_texture(m_node_type_manager);
}

registerer::~registerer()
{
}

}} // namespace pt::ui
