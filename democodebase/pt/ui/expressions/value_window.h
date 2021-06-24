#ifndef PT_UI_ELEMENTS_VALUE_WINDOW_H
#define PT_UI_ELEMENTS_VALUE_WINDOW_H

#include "../value_element.h"
#include "pt/win32.h"
#include "pt/boost/smart_ptr.h"
#include "pt/expressions/values/basic.h"

namespace pt { namespace ui { namespace elements {

class window;

/// UI window element expression value. Uses lazy evaluation.
/// ui::elements::window is created when primitive() is called.
/// ui::elements::window has no dependency to expressions.
class value_window : public value_element
{
public:
    value_window(shared_ptr<pt::expressions::value_string> class_name,
        shared_ptr<pt::expressions::value_string> window_name,
        shared_ptr<pt::expressions::value_int> width,
        shared_ptr<pt::expressions::value_int> height,
        shared_ptr<pt::expressions::value_bool> size_includes_borders,
        shared_ptr<value_element> content);

    /// Performs dynamic casts for inputs.
    static shared_ptr<value_window> create(
        std::vector<shared_ptr<expressions::value> > const& inputs);

public: // interface required by value
    std::string human_readable_text() const;

public: // interface required by expressions::value_primitive_ptr
    shared_ptr<bounded_element> get_primitive() const;

private: // data members
    mutable shared_ptr<window> m_window;

    shared_ptr<pt::expressions::value_string> m_class_name;
    shared_ptr<pt::expressions::value_string> m_window_name;
    shared_ptr<pt::expressions::value_int> m_width;
    shared_ptr<pt::expressions::value_int> m_height;
    shared_ptr<pt::expressions::value_bool> m_size_includes_borders;
    shared_ptr<value_element> m_content;
};

/// Evaluates given value_window.
class value_window_launcher : public pt::expressions::value
{
public:
    value_window_launcher(shared_ptr<value_window> window);

public: // interface required by value
    std::string human_readable_text() const
    {
        return "ui.window_launcher";
    }

private: // expressions::value virtual function implementations
    shared_ptr<pt::expressions::type> get_type() const;
};

void register_values_ui_window(
    expressions::function_registration_manager& manager);

}}} // namespace pt::ui::elements

#endif
