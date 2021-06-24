#ifndef PT_UI_ELEMENTS_VALUE_TEXTURE_H
#define PT_UI_ELEMENTS_VALUE_TEXTURE_H

#include "pt/expressions/values/primitive.h"
#include "pt/boost/noncopyable.h"
#include "pt/boost/path.h"
#include "pt/expressions/type_primitive.h"
#include "pt/expressions/values/basic.h"
#include "pt/ui/expressions/value_graphics_device.h"

namespace pt { namespace dx {
    class graphics_device;
    class texture_2d;
}} // namespace pt::expressions

namespace pt { namespace expressions {
    class function_registration_manager;
}} // namespace pt::expressions

namespace pt { namespace ui { namespace elements {

class value_texture
:   public pt::expressions::values::primitive_ptr<dx::texture_2d>
,   noncopyable
{
public:
    value_texture(shared_ptr<ui::expressions::value_graphics_device> device,
        shared_ptr<pt::expressions::value_string> path);

    static shared_ptr<value_texture> create(
        shared_ptr<pt::expressions::value> device,
        shared_ptr<pt::expressions::value> path);

public: // interface required by value
    std::string human_readable_text() const
    {
        return "ui.texture";
    }

public: // interface required by expressions::value_primitive_ptr
    shared_ptr<dx::texture_2d> get_primitive() const;
    void set_primitive(shared_ptr<dx::texture_2d> primitive);

private: // virtual function implementations for expressions::value
    shared_ptr<pt::expressions::type> get_type() const;

private: // data members
    shared_ptr<ui::expressions::value_graphics_device> m_device;
    shared_ptr<pt::expressions::value_string> m_path;

    mutable shared_ptr<dx::texture_2d> m_texture;
};

typedef pt::expressions::type_primitive<dx::texture_2d> type_texture_2d;

void register_values_ui_texture(
    pt::expressions::function_registration_manager& manager);

}}} // namespace pt::ui::elements

#endif
