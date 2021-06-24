#include "draw_to_texture.h"
#include "rendertarget.h"
#include "../utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/texture_2d_value.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/format.h"
#include "pt/log.h"
#include "pt/math_utils.h"
#include "pt/parameter_collection.h"
#include "pt/parameter_node.h"
#include "pt/vector_utility.h"

namespace content { namespace sequences {

draw_to_texture::draw_to_texture(d3d9::graphics_device& device,
    config_node const& config,
    parameter_collection const& shared_parameters)
: m_basic(config, shared_parameters)
, m_is_clear_color_enabled(true)
{
    assert(config.key() == "draw_to_texture");

    std::shared_ptr<parameter_node> curves
        = optional_child_node("curves", shared_parameters);

    m_clear_color = create_optional_curve_value<vector4f>(config, "clear_color",
        curves, vector4f(0.0f, 0.0f, 0.0f, 1.0f));

    config.parse_optional_child("clear_color_enabled", m_is_clear_color_enabled, true);
    log(pt::format("draw_to_texture '%1%' clear_color_enabled '%2%'") % config.child("name")->value() % m_is_clear_color_enabled);

    m_texture = find_shared_texture(config, shared_parameters, "shared_texture");
    if (m_texture)
        return;

    // Rest is only about initializing m_texture. NOTE the early return above.

    config.parse_child("resolution", m_resolution);
    config.parse_optional_child("bits_per_channel", m_bits_per_channel, 8);

    UINT width = m_resolution[0];
    UINT height = m_resolution[1];
    UINT levels = 1;
    DWORD usage = D3DUSAGE_RENDERTARGET;
    D3DFORMAT format = D3DFMT_X8R8G8B8;
    D3DPOOL pool = D3DPOOL_DEFAULT;

    switch(m_bits_per_channel)
    {
    case 8:
        format = D3DFMT_A8R8G8B8;
        break;

    case 16:
        format = D3DFMT_A16B16G16R16F;
        break;

    case 32:
        format = D3DFMT_A32B32G32R32F;
        break;

    default:
        throw any_error("given bits per channel not implemented yet");
    }

    m_texture = d3d9::texture_2d_builder(device)
        .width(width)
        .height(height)
        .levels(levels)
        .usage(usage)
        .format(format)
        .pool(pool)
        .build_shared();
}

draw_to_texture::draw_to_texture(config_node const& config,
	std::shared_ptr<d3d9::texture_2d> texture)
: m_basic(config)
, m_texture(texture)
, m_is_clear_color_enabled(true)
{
    assert(config.key() == "draw_to_texture");
}

string_wrapper const& draw_to_texture::name() const
{
    return m_basic.name();
}

std::shared_ptr<d3d9::texture_2d> draw_to_texture::draw(rendertarget&,
    float time)
{
    d3d9::scoped_render_target_texture scoped_target(*m_texture);

    if (m_is_clear_color_enabled)
    {
        // Clear.
        DWORD flags = D3DCLEAR_TARGET;
        //if (m_depth_stencil)
        //flags |= D3DCLEAR_ZBUFFER;
        //vector4f clear_color(0, 0, 0.25f, 0);

        //vector4f clear_color(0, 0, 0, 1.0f);

        vector4f clear_color(m_clear_color->value_at(time));
        clear_color[0] = pt::clamp(clear_color[0], 0.0f, 1.0f);
        clear_color[1] = pt::clamp(clear_color[1], 0.0f, 1.0f);
        clear_color[2] = pt::clamp(clear_color[2], 0.0f, 1.0f);
        clear_color[3] = pt::clamp(clear_color[3], 0.0f, 1.0f);

        d3d9::clear(m_texture->device(), flags, clear_color, 1.0f, 0);
    }

    d3d9::scoped_render_state culling(m_texture->device(), D3DRS_CULLMODE, D3DCULL_CCW);
    d3d9::scoped_render_state zenable(m_texture->device(), D3DRS_ZENABLE, TRUE);

    //d3d9::scoped_texture_stage_state color_op(m_texture->device(), 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    //d3d9::scoped_texture_stage_state alpha_op(m_texture->device(), 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    //d3d9::scoped_render_state lightning(m_texture->device(), D3DRS_LIGHTING, FALSE);

    //d3d9::set_render_state(m_texture->device(), D3DRS_ALPHABLENDENABLE, FALSE);
    //d3d9::set_render_state(m_texture->device(), D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
    //d3d9::set_render_state(m_texture->device(), D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

    // Draw
    rendertarget new_target(m_texture->device(), m_texture);

    m_basic.draw(new_target, time);

    return m_texture;
}

float draw_to_texture::start_time() const
{
    return m_basic.start_time();
}

float draw_to_texture::end_time() const
{
    return m_basic.end_time();
}

void draw_to_texture::set_override_view(d3d9::rendering::item* view)
{
    m_basic.set_override_view(view);
}

void draw_to_texture::link_inputs(composite_sequence_collection const& sequences)
{
    m_basic.link_inputs(sequences);
}

// free functions

std::shared_ptr<d3d9::texture_2d> find_shared_texture(config_node const& config,
    parameter_collection const& shared_parameters, std::string const& xml_element_name)
{
	std::shared_ptr<d3d9::texture_2d> result;

    config_node::const_iterator shared_config_iter
        = config.find_child(xml_element_name);

    if (shared_config_iter != config.end())
    {
        std::string texture_name;
        shared_config_iter->parse_child("name", texture_name);

        parameter_collection::const_iterator curves_iter
            = shared_parameters.child("curves");

        parameter_collection::const_iterator shared_iter
            = (*curves_iter)->children().find_child(texture_name);

        if (shared_iter != (*curves_iter)->children().end())
        {
            std::shared_ptr<animatable_value_base> value_base
                = (*shared_iter)->value();
            assert(value_base);

            std::shared_ptr<d3d9::texture_2d_value> value
                = std::dynamic_pointer_cast<d3d9::texture_2d_value>(value_base);
            assert(value);

            result = value->texture_source()->value_at(0);
        }
        else
        {
            log(format("Shared texture '%1%' not found") % texture_name);
        }
    }

    return result;
}

}} // namespace content::sequences
