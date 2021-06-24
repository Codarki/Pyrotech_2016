#ifndef PT_UI_NODE_VISUALIZER_H
#define PT_UI_NODE_VISUALIZER_H

#include "pt/operations/node.h"
#include "pt/operations/nodes/basic_instance.h"
//#include "pt/operations/node_type.h"

namespace pt { namespace operations {
    class node_type_registration_manager;
}} // namespace pt::operations

namespace pt { namespace ui {

class texture_visualizer_node : public operations::node
{
public:
    texture_visualizer_node(std::string const& id,
        operations::nodes::type const* node_type);

private: // virtual function implementations required by node
    void do_load(config_node const& config);
    void do_save(config_node& config) const;
    shared_ptr<operations::nodes::instance> do_create_instance();
};

class texture_visualizer : public operations::nodes::basic_instance
{
public:
    texture_visualizer(operations::node& node);

public: // interface required by node_instance
    shared_ptr<operations::nodes::editor> editor();
    void handle_event(ui::event& event,
        operations::nodes::event_context& context);

private: // data members
    operations::node& m_node;
    shared_ptr<operations::input_pin_instance> m_input_pin_instance;
};

void register_ui_node_types_visualizer(
    operations::node_type_registration_manager& manager);

}} // namespace pt::ui

#endif
