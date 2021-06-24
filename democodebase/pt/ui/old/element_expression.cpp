#include "element_expression.h"
#include "value_element.h"
#include "pt/expressions/value_none.h"
#include "pt/expressions/value_factory.h"

namespace pt { namespace ui {

element_expression::element_expression(expressions::type_ptr const&)
{
}

size_t element_expression::child_count() const
{
    return m_childs.size();
}

void element_expression::set_child_count(size_t count)
{
    m_childs.resize(count);
}

void element_expression::set_child(size_t index,
    shared_ptr<element_expression> const& child)
{
    assert(index < child_count());
    m_childs[index] = child;
}

shared_ptr<value_element> element_expression::evaluate_element(
    expressions::environment_ptr const& environment)
{
    if(!m_element)
    {
        std::vector<expressions::value_ptr> child_values(m_childs.size());

        for(size_t i = 0; i < m_childs.size(); ++i)
        {
            child_values[i] = m_childs[i]->evaluate_element(environment);

            assert(child_values[i]);
        }

        m_element = create_value_element("ui.window", type_element::instance(),
            child_values);
    }
    assert(m_element);
    return m_element;
}

expressions::value_ptr element_expression::evaluate(
    expressions::environment_ptr const& environment)
{
    return evaluate_element(environment);
}

std::set<expressions::symbol> element_expression::depends_on() const
{
    return std::set<expressions::symbol>();
}

}} // namespace pt::ui
