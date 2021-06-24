#include "utility.h"
#include "../application.h"
#include "curve_free_functions.h"
#include "pt/animatable_value_utility.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/parameter_node_utility.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/path.h"

#include "../box_vertex_stream.h"
#include "pt/d3d9/rendering/drawable_builder.h"
#include "pt/d3d9/rendering/basic_geometry.h"
#include "pt/d3d9/rendering/item_geometry.h"
#include "pt/d3d9/rendering/material_cache.h"
#include "pt/d3d9/rendering/light.h"
#include "pt/d3d9/rendering/scene.h"

#include "pt/d3d9/vertex_streams.h"
#include "pt/d3d9/constant_texture_2d_source.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/d3d9/texture_2d_value.h"

#include "pt/d3d9/graphics_device.h"
#include "pt/boost/lexical_cast.h"

namespace content {

std::shared_ptr<parameter_node> parse_curve_parameters(pt::path const& path)
{
    std::shared_ptr<parameter_node> result(new parameter_node("curves"));

    config_node config(path);

    // Create curves
    config_node::const_iterator curves = config.child("curves");
    for (config_node::const_iterator i = curves->begin();
        i != curves->end(); ++i)
    {
        result->children().add(create_spline(*i));
    }

    // Create reference curves from existing curves
    for (config_node::const_iterator i = curves->begin();
        i != curves->end(); ++i)
    {
        auto existing_curves = result->children();
        auto curve = create_optional_reference_curve(*i, existing_curves);
        if (curve)
            result->children().add(curve);
    }

    return result;
}

D3DFORMAT d3d_format(int bpp)
{
    switch (bpp)
    {
    case 8:
        return D3DFMT_A8R8G8B8;

    case 16:
        return D3DFMT_A16B16G16R16F;

    case 32:
        return D3DFMT_A32B32G32R32F;

    case 160:
        return D3DFMT_R16F;

    case 320:
        return D3DFMT_R32F;

    default:
        throw any_error("given bits per channel not implemented yet");
    }
}

std::shared_ptr<d3d9::texture_2d> create_rendertarget_texture2d(d3d9::graphics_device& device, D3DPOOL pool, int width, int height, int bpp)
{
    UINT levels = 1;
    std::shared_ptr<d3d9::texture_2d> texture
        = d3d9::texture_2d_builder(device)
        .width(width)
        .height(height)
        .levels(levels)
        .usage(D3DUSAGE_RENDERTARGET)
        .format(d3d_format(bpp))
        .pool(pool)
        .build_shared();

    return texture;
}

std::shared_ptr<d3d9::texture_2d> create_and_add_lockable_texture2d(d3d9::graphics_device& device, std::string name, int width, int height, int bpp)
{
    UINT levels = 1;
    std::shared_ptr<d3d9::texture_2d> texture
        = d3d9::texture_2d_builder(device)
        .width(width)
        .height(height)
        .levels(levels)
        .usage(D3DUSAGE_DYNAMIC)
        .format(d3d_format(bpp))
        .pool(D3DPOOL_SYSTEMMEM)
        .build_shared();

    d3d9::default_texture_2d_collection().add(name, texture);

    log(pt::format("Created and added lockable \"%1%\"") % name);

    return texture;
}

std::shared_ptr<d3d9::texture_2d> create_and_add_rendertarget_texture2d(d3d9::graphics_device& device, std::string name, int width, int height, int bpp)
{
    auto texture = create_rendertarget_texture2d(device, D3DPOOL_DEFAULT, width, height, bpp);

    d3d9::default_texture_2d_collection().add(name, texture);

    log(pt::format("Created and added rendertexture \"%1%\"") % name);

    return texture;
}

void parse_shared_textures(path const& path, parameter_collection& target,
    d3d9::graphics_device& device)
{
    try
    {
        config_node config(path);

        config_node::const_iterator textures
            = config.child("shared_textures");
        for (config_node::const_iterator i = textures->begin();
            i != textures->end(); ++i)
        {
            assert(i->key() == "texture");

            std::string name;
            vector2i resolution;

            i->parse_child("name", name);
            //i->parse_child("resolution", resolution);

            if (i->child("resolution")->value() == "screen")
            {
                resolution = content::application::resolution();
            }
            else if (i->child("resolution")->value() == "half_backbuffer")
            {
                
                D3DPRESENT_PARAMETERS present_params = device.parameters();
                resolution[0] = present_params.BackBufferWidth / 2;
                resolution[1] = present_params.BackBufferHeight / 2;
            }
            else if (i->child("resolution")->value() == "backbuffer")
            {
                
                D3DPRESENT_PARAMETERS present_params = device.parameters();
                resolution[0] = present_params.BackBufferWidth;
                resolution[1] = present_params.BackBufferHeight;
            }
            else
            {
                i->parse_child("resolution", resolution);
            }

            std::string bits_per_channel;
            i->parse_optional_child("bits_per_channel", bits_per_channel, std::string("8"));

            int bpp = 8;

            if (bits_per_channel == "depth_32")
            {
                bpp = 320;
            }
            else if (bits_per_channel == "depth_16")
            {
                bpp = 160;
            }
            else
            {
                bpp = boost::lexical_cast<int>(bits_per_channel);
            }

            bool is_lockable;
            i->parse_optional_child("is_lockable", is_lockable, false);

            std::shared_ptr<d3d9::texture_2d> texture;
            if (is_lockable)
            {
                texture = create_and_add_lockable_texture2d(device, name, resolution[0], resolution[1], bpp);
            }
            else
            {
                texture = create_and_add_rendertarget_texture2d(device, name, resolution[0], resolution[1], bpp);
            }

            std::shared_ptr<d3d9::constant_texture_2d_source> source(
                new d3d9::constant_texture_2d_source(texture));

            std::shared_ptr<animatable_value_base> texture_value(
                new d3d9::texture_2d_value(std::move(source)));

            std::shared_ptr<parameter_node> node(
                new parameter_node(name, texture_value));

            log(pt::format("Added shared texture \"%1%\"") % name);

            target.add(node);
        }
    }
    catch(std::exception& e)
    {
        log(std::string("Failed to load shared textures: ") + e.what());
        return;
    }
}

config_node get_post_processing_parameter_config()
{
    config_node config("data/post_processing/parameters.xml");
    return *config.child("post_processing_parameters");
}

/// Sets a box geometry for all light items.
void set_light_boxes(d3d9::graphics_device& device,
    d3d9::rendering::scene& scene,
    d3d9::rendering::material_cache& materials)
{
    std::shared_ptr<box_vertex_stream> box(new box_vertex_stream(device, 10.0f));

    std::shared_ptr<d3d9::vertex_streams> streams(
        new d3d9::vertex_streams(device));
    streams->add(box);

    std::shared_ptr<d3d9::rendering::surface> surface
        = materials.find_or_create(device, "valon materiaali",
        parameter_collection());

    auto box_geometry = std::make_shared<d3d9::rendering::basic_geometry>(
        d3d9::rendering::build_shared_basic_drawable(device, streams, box->indices()), surface);

    for (size_t i = 0; i < scene.light_count(); ++i)
    {
        // TODO: Dont give shared ownership of transformer to item_geometry.
        //d3d9::rendering::item& item_with_light = *scene.lights()[i];

        //std::unique_ptr<d3d9::rendering::item_geometry> item_geometry = d3d9::rendering::build_unique_item_geometry(
        //    box_geometry,
        //    item_with_light.transformer());

        //item_with_light.set_geometry(move(item_geometry));
    }
}

} // namespace content
