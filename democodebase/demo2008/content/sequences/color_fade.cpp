#include "color_fade.h"
#include "rendertarget.h"
#include "../utility.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/exceptions/string_context.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/vector_utility.h"
#include "pt/math_utils.h"
#include "pt/parameter_collection.h"

namespace pt {
    void parse_from_text(content::sequences::color_fade::fade_type& value,
        std::string const& text)
    {
        if (text == "cos_180_curve")
            value = content::sequences::color_fade::cos_180_curve;
        else if (text == "pow_4_curve")
            value = content::sequences::color_fade::pow_4_curve;
        else if (text == "inv_pow_4_curve")
            value = content::sequences::color_fade::inv_pow_4_curve;
        else if (text == "linear")
            value = content::sequences::color_fade::linear;
        else
        {
            throw any_error(
                "invalid curve type. Available: cos_180_curve, pow_4_curve, "
                "inv_pow_4_curve, linear");
        }
    }
} // namespace pt

namespace content { namespace sequences {

namespace {
    char vertex[] =
        "struct input_vertex\n"
        "{\n"
        "    float4 position : POSITION;\n"
        "    float2 texcoord : TEXCOORD0;\n"
        "};\n"

        "void main_vs(inout input_vertex input)\n"
        "{\n"
        "}\n";

    char pixel[] =
        "float3 color;\n"
        "float alpha;\n"

        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 out_color : COLOR0)\n"
        "{\n"
        "    out_color.xyz = color;\n"
        "    out_color.w = alpha;\n"
        "}\n"
        ;
} // anonymous namespace

color_fade::color_fade(d3d9::graphics_device& device,
    config_node const& config, parameter_collection const& parameters)
: simple_composite_sequence(config)
, m_device(&device)
, m_time(0.0f)
{
    assert(config.key() == "color_fade");

	// Why is duration animated? makes no sense.

    std::shared_ptr<parameter_node> curves = *parameters.child("curves");

    config.parse_child("fade_in_duration", m_fade_in_duration);
    config.parse_child("fade_out_duration", m_fade_out_duration);
    config.parse_optional_child("max_alpha", m_max_alpha, 1.0f);
    config.parse_optional_child("type", m_type, cos_180_curve);

    m_fade_in_duration = std::max(0.0f, m_fade_in_duration);
    m_fade_out_duration = std::max(0.0f, m_fade_out_duration);
    m_max_alpha = pt::clamp(m_max_alpha, 0.0f, 1.0f);

    m_duration = create_curve_value<float>(
        *config.child("total_duration"), curves);
    m_color = create_curve_value<vector3f>(*config.child("color"), curves);

    // WTF.
    set_end_time(m_duration->value_at(m_time));

    try
    {
        m_pixel_shader = d3d9::compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = d3d9::compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(d3d9::compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("color_fade"));
        throw;
    }
}

namespace {
    float do_cos_curve_180(float t)
    {
        if (t < 0.0f || t > 1.0f)
            return t;

        return -std::cos(t * 3.1415f) * 0.5f + 0.5f;
    }
    float do_cos_curve_90_upwards(float t)
    {
        if (t < 0.0f || t > 1.0f)
            return t;

        return 1.0f - std::cos(t * 3.1415f * 0.5f);
    }
    float do_cos_curve_90_downwards(float t)
    {
        if (t < 0.0f || t > 1.0f)
            return t;

        return std::sin(t * 3.1415f * 0.5f);
    }
    float do_pow_4_curve(float t)
    {
        if (t < 0.0f || t > 1.0f)
            return t;

        return std::pow(t, 4.0f);
    }
    float do_inv_pow_4_curve(float t)
    {
        if (t < 0.0f || t > 1.0f)
            return t;

        return 1.0f - std::pow(1.0f - t, 4.0f);
    }
} // anonymous namespace

std::shared_ptr<d3d9::texture_2d> color_fade::on_draw(rendertarget& target,
    float time, std::shared_ptr<d3d9::texture_2d> child_result)
{
    m_time = time;

    // Draw color to \a target.
    scoped_optional_rendertarget scoped_target(target);

    m_pixel_shader->use();
    m_vertex_shader->use();

    float fade_in = m_fade_in_duration
        ? m_time / m_fade_in_duration : 1.0f;
    float fade_out = m_fade_out_duration
        ? (m_duration->value_at(m_time) - m_time) / m_fade_out_duration : 1.0f;

    float alpha = std::min(1.0f, std::min(fade_in, fade_out));

    if (alpha < 0.0f)
        return nullptr;

    switch(m_type)
    {
    case cos_180_curve:
        alpha = do_cos_curve_180(alpha);
        break;

    case pow_4_curve:
        alpha = do_pow_4_curve(alpha);
        break;

    case inv_pow_4_curve:
        alpha = do_inv_pow_4_curve(alpha);
        break;
    }
    //assert(m_type == linear);

    alpha *= m_max_alpha;

    if (m_pixel_shader->constants().count() > 0)
    {
        vector4f color(m_color->value_at(m_time), 1.0f);
        color[0] = pt::clamp(color[0], 0.0f, 1.0f);
        color[1] = pt::clamp(color[1], 0.0f, 1.0f);
        color[2] = pt::clamp(color[2], 0.0f, 1.0f);
        color[3] = pt::clamp(color[3], 0.0f, 1.0f);

        d3d9::constant const& c1 = m_pixel_shader->constants().by_name("alpha");
        d3d9::constant const& c2 = m_pixel_shader->constants().by_name("color");
        d3d9::set_constant(c1, m_pixel_shader->constants(), alpha);
        d3d9::set_constant(c2, m_pixel_shader->constants(), color);
    }

    d3d9::graphics_device& device = *m_device;

    d3d9::scoped_render_state blend(device, D3DRS_ALPHABLENDENABLE, TRUE);
    d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    d3d9::rendering::fill_fullscreen_quad(device);

    return child_result;
}

}} // namespace content::sequences
