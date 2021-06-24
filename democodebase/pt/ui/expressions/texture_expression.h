#ifndef PT_UI_EXPRESSIONS_TEXTURE_EXPRESSION_H
#define PT_UI_EXPRESSIONS_TEXTURE_EXPRESSION_H

#include "pt/expressions/basic_expression.h"

namespace pt { namespace ui { namespace elements {
    class value_texture;
}}} // namespace pt::ui::elements

namespace pt { namespace ui { namespace expressions {

/// Expression which evaluates to elements::value_texture.
/// Depends on environment symbol "graphics_device".
class texture_expression : public pt::expressions::basic_expression
{
public:
    texture_expression();

    shared_ptr<elements::value_texture> evaluate_texture(
        shared_ptr<pt::expressions::environment const> environment) const;

public: // interface required by expression
    std::string human_readable_text() const;

private: // cached_expression virtual function implementations
    shared_ptr<pt::expressions::value> evaluate_cached(
        shared_ptr<pt::expressions::environment const> environment) const;

private: // helper functions
    shared_ptr<elements::value_texture> do_evaluate_texture(
        shared_ptr<pt::expressions::environment const> environment) const;

private: // data members
    shared_ptr<pt::expressions::expression> m_device_from_environment;
};

}}} // namespace pt::ui::expressions

#endif
