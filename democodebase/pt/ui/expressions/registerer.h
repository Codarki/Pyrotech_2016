#ifndef PT_UI_REGISTERER_H
#define PT_UI_REGISTERER_H

#include "element_registration_manager.h"
#include "pt/expressions/function_registration_manager.h"
#include "pt/operations/node_type_registration_manager.h"

namespace pt { namespace ui {

/// Registers pt::ui project classes on construction and unregisters on
/// destruction.
class registerer
{
public:
    registerer(shared_ptr<pt::expressions::function_registry> functions,
        shared_ptr<operations::node_type_registry> node_types);
    ~registerer();

private: // data members
    element_registration_manager m_element_registration_manager;
    expressions::function_registration_manager m_function_registration_manager;
    operations::node_type_registration_manager m_node_type_manager;
};

}} // namespace pt::ui

#endif
