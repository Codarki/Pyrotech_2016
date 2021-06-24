#include "image.h"
#include "rendertarget.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/format.h"

namespace content { namespace sequences {

image::image(d3d9::graphics_device& device, std::string const& name,
    pt::path const& path)
: simple_composite_sequence(name)
{
    m_texture = d3d9::default_texture_2d_collection().find(path.string());

    if (!m_texture)
        m_texture = d3d9::load_immutable_texture_2d(device, path);

    d3d9::default_texture_2d_collection().add(path.string(), m_texture);
}

image::image(d3d9::graphics_device& device, config_node const& config)
: simple_composite_sequence(config)
{
    assert(config.key() == "image");

    std::string image_name;
    config.parse_child("path", image_name);
    config.parse_child("name", m_name);

    m_texture = d3d9::default_texture_2d_collection().find(image_name);
    if (m_texture)
        return;

    m_texture = d3d9::load_texture_2d(device, image_name);

    d3d9::default_texture_2d_collection().add(image_name, m_texture);
}

std::shared_ptr<d3d9::texture_2d const> image::texture() const
{
    return m_texture;
}

std::shared_ptr<d3d9::texture_2d> image::on_draw(rendertarget&,
    float, std::shared_ptr<d3d9::texture_2d> child_result)
{
    if (child_result)
    {
        throw any_error(
            format("'%1%': child input result detected, but not supported")
            % name());
    }

    return m_texture;
}

}} // namespace content::sequences
