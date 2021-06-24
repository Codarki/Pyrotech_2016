#ifndef PT_UI_ELEMENT_EXPRESSION_H
#define PT_UI_ELEMENT_EXPRESSION_H

#include "pt/expressions/expression.h"
#include "pt/std/vector.h"

namespace pt { namespace expressions {
    class type;
    typedef shared_ptr<expressions::type> type_ptr;
}} // namespace pt::expressions

namespace pt { namespace ui {

class value_element;

class element_expression : public expressions::expression
{
public:
    element_expression(expressions::type_ptr const& type);

    size_t child_count() const;
    void set_child_count(size_t count);
    void set_child(size_t index, shared_ptr<element_expression> const& child);

    shared_ptr<value_element> evaluate_element(
        expressions::environment_ptr const& environment);

public: // interfafce required by expressions::expression
    expressions::value_ptr evaluate(
        expressions::environment_ptr const& environment);
    std::set<expressions::symbol> depends_on() const;

private: // type definitions
    typedef std::vector<shared_ptr<element_expression> > child_container;

private: // data members
    child_container m_childs;
    shared_ptr<value_element> m_element;
};

}} // namespace pt::ui

#endif
