#include "value_window.h"
#include "window.h"
#include "../event_handler.h"
#include "pt/log.h"
#include "pt/boost/bind.h"
#include "pt/boost/format.h"
#include "pt/expressions/function_registration_manager.h"
#include "pt/expressions/function_signature.h"
#include "pt/expressions/type_none.h"

namespace pt { namespace ui { namespace elements {

value_window::value_window(
    shared_ptr<pt::expressions::value_string> class_name,
    shared_ptr<pt::expressions::value_string> window_name,
    shared_ptr<pt::expressions::value_int> width,
    shared_ptr<pt::expressions::value_int> height,
    shared_ptr<pt::expressions::value_bool> size_includes_borders,
    shared_ptr<value_element> content)
:   m_class_name(class_name)
,   m_window_name(window_name)
,   m_width(width)
,   m_height(height)
,   m_size_includes_borders(size_includes_borders)
,   m_content(content)
{
}

shared_ptr<value_window> value_window::create(
    std::vector<shared_ptr<expressions::value> > const& inputs)
{
    assert(inputs.size() == 6);

    using expressions::value_string;
    using expressions::value_int;
    using expressions::value_bool;

    shared_ptr<value_string> class_name = dynamic_pointer_cast<value_string>(
        inputs[0]);
    shared_ptr<value_string> name = dynamic_pointer_cast<value_string>(
        inputs[1]);
    shared_ptr<value_int> width = dynamic_pointer_cast<value_int>(inputs[2]);
    shared_ptr<value_int> height = dynamic_pointer_cast<value_int>(inputs[3]);
    shared_ptr<value_bool> size_includes_borders =
        dynamic_pointer_cast<value_bool>(inputs[4]);
    shared_ptr<value_element> content = dynamic_pointer_cast<value_element>(
        inputs[5]);

    return shared_ptr<value_window>(new value_window(class_name, name, width,
        height, size_includes_borders, content));
}

std::string value_window::human_readable_text() const
{
    return "ui.window";
}

shared_ptr<bounded_element> value_window::get_primitive() const
{
    if(!m_window)
    {
        log("Creating ui::elements::window...");

        m_window.reset(new window(m_class_name->get_primitive()));

        if(m_size_includes_borders->get_primitive())
        {
            m_window->create(m_window_name->get_primitive(),
                m_width->get_primitive(), m_height->get_primitive());
        }
        else
        {
            m_window->create_with_client_size(m_window_name->get_primitive(),
                m_width->get_primitive(), m_height->get_primitive());
        }

        bool fullscreen = false;

        shared_ptr<event_handler> handler
            = dynamic_pointer_cast<event_handler>(m_content->get_primitive());
        assert(handler);

        m_window->init_direct3d(fullscreen);
        m_window->set_content(m_content->get_primitive());
        m_window->set_handler(handler);
        m_content->get_primitive()->set_parent(m_window.get());
    }
    return m_window;
}

value_window_launcher::value_window_launcher(
    shared_ptr<value_window> window)
{
    // evaluate..
    window->get_primitive();
}

shared_ptr<pt::expressions::type> value_window_launcher::get_type() const
{
    return expressions::type_none::instance();
}

namespace {
    expressions::value_ptr create_window_element(
        expressions::type_const_ptr const&,
        std::vector<expressions::value_ptr> const& inputs)
    {
        return value_window::create(inputs);
    }

    expressions::value_ptr create_window_launcher(
        expressions::type_const_ptr const&,
        std::vector<expressions::value_ptr> const& inputs)
    {
        shared_ptr<value_window> window
            = dynamic_pointer_cast<value_window>(inputs[0]);

        return expressions::value_ptr(
            new value_window_launcher(window));
    }

    void register_value_window_launcher(
        expressions::function_registration_manager& manager)
    {
        expressions::function_signature sig("ui.window_launcher",
            "UI window launcher.", pt::expressions::type_none::instance());
        sig.set_input_count(1);
        sig.set_input(0, expressions::function_input("window",
            "Window to be launched.", type_element::instance()));

        manager.add(sig, bind(create_window_launcher, _1, _2));
    }
} // anonymous namespace

void register_values_ui_window(
    expressions::function_registration_manager& manager)
{
    expressions::function_signature sig("ui.window",
        "UI window element.", type_element::instance());
    sig.set_input_count(6);
    sig.set_input(0, expressions::function_input("class_name",
        "Name for the window class.", expressions::type_string::instance()));
    sig.set_input(1, expressions::function_input("name",
        "Name of the window.", expressions::type_string::instance()));
    sig.set_input(2, expressions::function_input("width",
        "Width in pixels.", expressions::type_int::instance()));
    sig.set_input(3, expressions::function_input("height",
        "Height in pixels.", expressions::type_int::instance()));
    sig.set_input(4, expressions::function_input("size_includes_borders",
        "\\c true if size includes window borders.",
        expressions::type_bool::instance()));
    sig.set_input(5, expressions::function_input("content",
        "Contents for the window.", type_element::instance()));

    manager.add(sig, bind(create_window_element, _1, _2));

    register_value_window_launcher(manager);
}

}}} // namespace pt::ui::elements
