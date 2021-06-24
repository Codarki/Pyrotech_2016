#include "node_visualizer.h"
#include "draw_event.h"
#include "elements/value_texture.h"
#include "pt/config_node.h"
#include "pt/vector_utility.h"
#include "pt/operations/input_pin.h"
#include "pt/operations/input_pin_instance.h"
#include "pt/operations/output_pin.h"
#include "pt/operations/node_type_registration_manager.h"
#include "pt/operations/nodes/basic_type.h"
#include "pt/operations/nodes/convenience.h"
#include "pt/operations/nodes/event_context.h"
#include "pt/operations/pins/type.h"
#include "pt/expressions/expression.h"
#include "pt/expressions/environment.h"
#include "pt/expressions/values/none.h"

namespace pt { namespace ui {

// texture_visualizer_node

texture_visualizer_node::texture_visualizer_node(std::string const& id,
    operations::nodes::type const* node_type)
:   node(id, node_type)
{
    operations::pins::container& input_pins
        = operations::nodes::get_input_pins(*this);

    input_pins.set_count(1);

    shared_ptr<pt::expressions::value> default_value
        = pt::expressions::values::none::instance();

    // Set default expression in input pin
    shared_ptr<pt::expressions::expression> null_expression;

    std::string const pin_name = "input";
    input_pins.push_back_working_pin(shared_ptr<operations::pin>(
        new operations::input_pin("input", null_expression)));

    visual_info().size[0] = 256;
    visual_info().size[1] = 256;

    set_pin_visual_infos();
}

void texture_visualizer_node::do_load(config_node const& config)
{
    std::string id;
    std::string name;
    config.parse_child("name", name);
    config.parse_child("id", id);

    std::string input_pin_name;
    std::string input_pin_default_value;
    config.child("input_pin")->parse_child("name", input_pin_name);

    std::string value;
    operations::nodes::visual_info info = visual_info();
    //config.parse_optional_child("input_count", input_count, 0);
    config.parse_optional_child("position", info.position);
    config.parse_optional_child("size", info.size);
    set_visual_info(info);

    assert(id == node::id());
}

void texture_visualizer_node::do_save(config_node& config) const
{
    write_child(config, "name", type().name());
    write_child(config, "id", node::id());

    operations::nodes::visual_info const& info = visual_info();
    write_child(config, "position", info.position);
    write_child(config, "size", info.size);

    for(operations::pins::collection::const_iterator i = pins().begin();
        i != pins().end(); ++i)
    {
        if(i->first != operations::pins::type_input)
            continue;

        operations::pins::container const& pin_container = i->second;
        for(size_t pin_index = 0; pin_index < pin_container.working_count();
            ++pin_index)
        {
            shared_ptr<operations::pin const> pin
                = pin_container.at(pin_index);

            config_node pin_config;
            pin_config.set_key("input_pin");
            write_child(pin_config, "name", pin->name());
            config.insert_child(pin_config);
        }
    }
}

shared_ptr<operations::nodes::instance>
    texture_visualizer_node::do_create_instance()
{
    return shared_ptr<operations::nodes::instance>(
        new texture_visualizer(*this));
}

//

texture_visualizer::texture_visualizer(operations::node& node)
:   basic_instance(node)
,   m_node(node)
{
    operations::pins::container& input_pins
        = operations::nodes::get_input_pins(node);
    input_pins.set_count(1);

    //shared_ptr<operations::pin> input_pin = input_pins.at(0);
    //m_input_pin_instance
    //    = dynamic_pointer_cast<operations::input_pin_instance>(
    //    input_pin->create_instance());

    m_input_pin_instance = working_input_pin_instance(input_pins, 0);
}

shared_ptr<operations::nodes::editor> texture_visualizer::editor()
{
    return shared_ptr<operations::nodes::editor>();
}

void texture_visualizer::handle_event(ui::event& event,
    operations::nodes::event_context& context)
{
    if(draw_event* p = dynamic_cast<draw_event*>(&event))
    {
        operations::nodes::visual_info const& info = m_node.visual_info();
        vector2f top_left_position = static_vector_cast<float>(
            vector2i(info.position) - info.size / 2 + context.area_offset);
        vector2f size = static_vector_cast<float>(info.size);

        shared_ptr<pt::expressions::value> val
            = m_input_pin_instance->peer_or_default_expression()->evaluate(
            context.environment);

        shared_ptr<elements::value_texture> val_tex
            = dynamic_pointer_cast<elements::value_texture>(val);

        ui::fill_rectangle(*p, top_left_position,
            top_left_position + size, vector4f(255, 255, 255, 255));

        if(val_tex)
        {
            p->canvas().fill_rectangle(top_left_position,
                top_left_position + size, val_tex->get_primitive());
        }
    }
}

//

void register_ui_node_types_visualizer(
    operations::node_type_registration_manager& manager)
{
    shared_ptr<operations::nodes::type> type(
        new operations::nodes::basic_type<texture_visualizer_node>(
        "ui.texture_visualizer"));
    manager.add(type);

    //manager.add(shared_ptr<operations::node_type>(
    //    new texture_visualizer_node_type));
}

}} // namespace pt::ui
