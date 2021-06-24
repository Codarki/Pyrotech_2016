#include "texture_expression.h"
#include "../elements/value_texture.h"
#include "pt/any_error.h"
#include "pt/expressions/expression_signals.h"
#include "pt/expressions/invalid_input.h"
#include "pt/expressions/value_from_environment.h"
#include "pt/expressions/values/none.h"

namespace pt { namespace ui { namespace expressions {

texture_expression::texture_expression()
:   m_device_from_environment(new pt::expressions::value_from_environment(
        "graphics_device"))
{
    set_input_count(2);
    set_input_expression(1, m_device_from_environment);
}

shared_ptr<elements::value_texture> texture_expression::evaluate_texture(
    shared_ptr<pt::expressions::environment const> environment) const
{
    // Go through cached_expression, so we dont create texture on every
    // evaluation.
    shared_ptr<elements::value_texture> result
        = dynamic_pointer_cast<elements::value_texture>(
        evaluate(environment));
    assert(result);
    return result;
}

std::string texture_expression::human_readable_text() const
{
    return "texture_expression";
}

shared_ptr<pt::expressions::value> texture_expression::evaluate_cached(
        shared_ptr<pt::expressions::environment const> environment) const
{
    return do_evaluate_texture(environment);
}

shared_ptr<elements::value_texture> texture_expression::do_evaluate_texture(
    shared_ptr<pt::expressions::environment const> environment) const
{
    if(!input_expression(0))
        throw pt::expressions::invalid_input("no input");
    if(!input_expression(1))
        throw any_error("arar");

    shared_ptr<pt::expressions::value> input_0
        = input_expression(0)->evaluate(environment);
    shared_ptr<pt::expressions::value> input_1
        = input_expression(1)->evaluate(environment);

    if(input_0 == pt::expressions::values::none::instance())
        throw pt::expressions::invalid_input("input 0 null");
    if(input_1 == pt::expressions::values::none::instance())
        throw any_error("env prob");

    return elements::value_texture::create(input_1, input_0);
}

}}} // namespace pt::ui::expressions
