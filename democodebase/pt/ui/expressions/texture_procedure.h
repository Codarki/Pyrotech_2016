#ifndef PT_UI_EXPRESSIONS_TEXTURE_PROCEDURE_H
#define PT_UI_EXPRESSIONS_TEXTURE_PROCEDURE_H

#include "pt/expressions/typed_procedure.h"
#include "pt/expressions/typed_procedure_expression.h"

//namespace pt { namespace ui { namespace elements {
//    class value_texture;
//}}} // namespace pt::ui::elements

namespace pt { namespace ui { namespace expressions {

class texture_expression;

/// Procedure which owns a texture_expression.
typedef pt::expressions::typed_procedure<texture_expression>
    texture_procedure;

typedef pt::expressions::typed_procedure_type<texture_expression>
    type_texture_procedure;

/// Expression which evaluates to texture_procedure.
typedef pt::expressions::typed_procedure_expression<texture_expression>
    texture_procedure_expression;

//class texture_procedure : public pt::expressions::procedure_expression
//{
//public:
//    texture_procedure();
//
//private: // data members
//    shared_ptr<pt::expressions::expression> m_device_from_environment;
//};

}}} // namespace pt::ui::expressions

#endif
