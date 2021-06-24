#include "value_texture.h"
#include "pt/any_error.h"
#include "pt/boost/bind.h"
#include "pt/boost/path.h"
#include "pt/boost/filesystem/convenience.h"
#include "pt_dx/texture_2d.h"
#include "pt/expressions/values/basic.h"
#include "pt/expressions/function_registration_manager.h"
#include "pt/expressions/function_signature.h"

namespace pt { namespace ui { namespace elements {

value_texture::value_texture(
    shared_ptr<ui::expressions::value_graphics_device> device,
    shared_ptr<pt::expressions::value_string> path)
:   m_device(device)
,   m_path(path)
{
    if(m_path->get_primitive().empty())
        throw any_error("empty filename");

    filesystem::path arr(m_path->get_primitive(), filesystem::native);

    if(!filesystem::exists(filesystem::complete(arr)))
        throw any_error("file not found");

    //std::string path = m_path->primitive();
}

shared_ptr<value_texture> value_texture::create(
    shared_ptr<pt::expressions::value> device,
    shared_ptr<pt::expressions::value> path)
{
    using pt::expressions::value_string;
    using pt::expressions::values::basic_cast;

    shared_ptr<ui::expressions::value_graphics_device> value_device
        = dynamic_pointer_cast<ui::expressions::value_graphics_device>(device);
    shared_ptr<value_string> value_path
        = basic_cast<std::string>(path);

    assert(value_device);
    if(!value_path)
        throw any_error("invalid input");

    return shared_ptr<value_texture>(
        new value_texture(value_device, value_path));

}

shared_ptr<dx::texture_2d> value_texture::get_primitive() const
{
    if(!m_texture)
    {
        shared_ptr<dx::graphics_device> device = m_device->get_primitive();
        std::string path = m_path->get_primitive();

        shared_ptr<dx::texture_2d> tmp(new dx::texture_2d(*device));
        tmp->load(path);

        m_texture = tmp;
    }
    return m_texture;
}

void value_texture::set_primitive(shared_ptr<dx::texture_2d> primitive)
{
    m_texture = primitive;
}

shared_ptr<pt::expressions::type> value_texture::get_type() const
{
    return type_texture_2d::instance();
}

// Free functions.

namespace {
    shared_ptr<pt::expressions::value> create_texture(
        shared_ptr<pt::expressions::type const> const&,
        std::vector<shared_ptr<pt::expressions::value> > const& inputs)
    {
        assert(inputs.size() == 2);
        assert(inputs[0]);
        assert(inputs[1]);
        return value_texture::create(inputs[1], inputs[0]);
    }
} // anonymous namespace

void register_values_ui_texture(
    pt::expressions::function_registration_manager& manager)
{
    pt::expressions::function_signature sig("ui.texture",
        type_texture_2d::instance());
    pt::expressions::function_input input("filename",
        "Full path for the source file",
        pt::expressions::type_string::instance());

    sig.set_input_count(1);
    sig.set_input(0, input);

    manager.add(sig, bind(create_texture, _1, _2));
}

}}} // namespace pt::ui::elements
