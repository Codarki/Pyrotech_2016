#ifndef PT_UI_VALUE_ELEMENT_H
#define PT_UI_VALUE_ELEMENT_H

#include "bounded_element.h"
#include "pt/expressions/type.h"
#include "pt/expressions/values/primitive.h"
#include "pt/std/vector.h"

namespace pt { namespace expressions {
    class value_registration_manager;
}} // namespace pt::expressions

namespace pt { namespace ui {

class event;

/// Type class for an UI element.
class type_element : public expressions::type
{
public:
    static shared_ptr<expressions::type> instance();

private: // expressions::type virtual function implementations
    std::string const& get_name() const;
    shared_ptr<expressions::type const> get_base_type() const;
};

///// UI element that can be input or output in expression graphs.
//class value_element : public expressions::value, public bounded_element
//{
//private: // expressions::value virtual function implementations
//    expressions::type_ptr get_type() const;
//};

/// UI element that can be input or output in expression graphs.
class value_element
    : public expressions::values::primitive_ptr<bounded_element>
{
private: // expressions::value virtual function implementations
    shared_ptr<expressions::type> get_type() const;
};

// Free functions

/// \return New UI element defined by \a value_name, \a result_type and given
/// inputs in \a child_values. \relatesalso ui::value_element
//shared_ptr<value_element> create_value_element(
//    std::string const& value_name,
//    expressions::type_const_ptr const& result_type,
//    std::vector<expressions::value_ptr> const& child_values);

//void register_values_ui_element(
//    expressions::value_registration_manager& manager);

}} // namespace pt::ui

#endif
