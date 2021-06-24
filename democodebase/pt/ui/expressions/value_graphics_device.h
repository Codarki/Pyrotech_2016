#ifndef PT_UI_EXPRESSIONS_VALUE_GRAPHICS_DEVICE_H
#define PT_UI_EXPRESSIONS_VALUE_GRAPHICS_DEVICE_H

#include "pt/expressions/type_primitive.h"
#include "pt/expressions/values/simple_primitive_ptr.h"

namespace pt { namespace dx {
    class graphics_device;
}} // namespace pt::dx

namespace pt { namespace ui { namespace expressions {

typedef pt::expressions::type_primitive<dx::graphics_device>
    type_graphics_device;

class value_graphics_device
    : public pt::expressions::values::simple_primitive_ptr<dx::graphics_device>
{
public: // interface required by value
    std::string human_readable_text() const;

private: // value virtual function implementations
    shared_ptr<pt::expressions::type> get_type() const
    {
        return type_graphics_device::instance();
    }
};

}}} // namespace pt::ui::expressions

#endif
