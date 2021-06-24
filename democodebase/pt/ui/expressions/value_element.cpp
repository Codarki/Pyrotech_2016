#include "value_element.h"
#include "pt/expressions/value_creation_function.h"
#include "pt/expressions/function_registry.h"
#include "pt/boost/null_deleter.h"

namespace pt { namespace ui {

// type_element

shared_ptr<expressions::type> type_element::instance()
{
    static type_element singleton;
    return shared_ptr<expressions::type>(&singleton, null_deleter);
}

std::string const& type_element::get_name() const
{
    static std::string name("ui.element");
    return name;
}

shared_ptr<expressions::type const> type_element::get_base_type() const
{
    return shared_ptr<expressions::type const>();
}

// value_element

shared_ptr<expressions::type> value_element::get_type() const
{
    return type_element::instance();
}

// free functions

//shared_ptr<value_element> create_value_element(std::string const& value_name,
//    expressions::type_const_ptr result_type,
//    std::vector<expressions::value_ptr> const& childs)
//{
//    expressions::function_signature sig(value_name, result_type);
//
//    expressions::value_creation_function const& func
//        = expressions::function_registry::instance().get(sig);
//    assert(func);
//
//    expressions::value_ptr val = func(result_type, childs);
//    shared_ptr<value_element> elem = dynamic_pointer_cast<value_element>(val);
//    assert(elem);
//    return elem;
//}

//void register_values_ui_element(
//    expressions::value_registration_manager& manager)
//{
//    manager.add("ui.window", value_creation_function function);
//}

}} // namespace pt::ui
