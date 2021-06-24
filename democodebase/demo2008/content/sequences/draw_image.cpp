#include "draw_image.h"
#include "rendertarget.h"
#include "../utility.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/animatable_value.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/math_utils.h"
#include "pt/parameter_collection.h"
#include "pt/exceptions/string_context.h"
#include "pt/matrix_operations.h"


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
        "texture2D source_texture;\n"
        "sampler2D source_sampler;\n"
        "float alpha;\n"
        "float intensity;\n"

        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
        "    float4 original_color = tex2D(source_sampler, texcoord).xyzw;\n"
        "    color = original_color;\n"
        "    color.xyz *= intensity;\n"

        "    color.w = saturate(color.w) * alpha;\n"
        "}\n"
        ;
	char pixel_ignore_alpha_channel[] =
		"texture2D source_texture;\n"
		"sampler2D source_sampler;\n"
		"float alpha;\n"
		"float intensity;\n"

		"void main_ps(\n"
		"    float2 texcoord : TEXCOORD0,\n"
		"    out float4 color : COLOR0)\n"
		"{\n"
		"    float4 original_color = tex2D(source_sampler, texcoord).xyzw;\n"
		"    color = original_color;\n"
		"    color.xyz *= intensity;\n"

		"    color.w = alpha;\n"
		"}\n"
		;
} // anonymous namespace

draw_image::draw_image(d3d9::graphics_device& device,
    config_node const& config, parameter_collection const& parameters)
: simple_composite_sequence(config)
//:   m_alpha(1.0f)
//,   m_intensity(1.0f)
{
    parse(config, parameters);
    init(device);
}

draw_image::draw_image(d3d9::graphics_device& device,
    std::shared_ptr<composite_sequence> input, config_node const& config,
    parameter_collection const& parameters)
: simple_composite_sequence(config)
{
    parse(config, parameters);
    add_child(input);
    init(device);
}

std::shared_ptr<d3d9::texture_2d> draw_image::on_draw(rendertarget& target,
    float time, std::shared_ptr<d3d9::texture_2d> child_result)
{
    m_time = time;

    float alpha = m_alpha->value_at(m_time);
    if (alpha < 0.0f)
        return nullptr;

    std::shared_ptr<d3d9::texture_2d> result = child_result;
    assert(result);

    // Draw image to \a target.
    scoped_optional_rendertarget scoped_target(target);

    d3d9::graphics_device& device = result->device();

    m_pixel_shader->use();
    m_vertex_shader->use();

    d3d9::set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    //d3d9::set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    //d3d9::set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    if (m_pixel_shader->constants().count() > 0)
    {
        float intensity = m_intensity->value_at(m_time);

        alpha = clamp(alpha, 0.0f, 1.0f);
        intensity = std::max(intensity, 0.0f);

        d3d9::constant const& c1 = m_pixel_shader->constants().by_name("alpha");
        d3d9::constant const& c2 = m_pixel_shader->constants().by_name("intensity");
        d3d9::set_constant(c1, m_pixel_shader->constants(), alpha);
        d3d9::set_constant(c2, m_pixel_shader->constants(), intensity);
    }

    d3d9::scoped_render_state alpha_state(device, D3DRS_ALPHABLENDENABLE, TRUE);
    d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    //d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, TRUE);
    //d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    //d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    d3d9::scoped_texture_stage_state color_op(device, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    d3d9::scoped_texture_stage_state alpha_op(device, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    d3d9::scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);

    d3d9::set_texture(*result, 0);
    //d3d9::rendering::fill_fullscreen_quad(device);

    vector2f center = m_center->value_at(m_time);
    vector2f size = m_size->value_at(m_time);

    vector2f min = center - (size * 0.5f);
    vector2f max = min + size;

    float rotation = m_rotation->value_at(m_time);

    if (std::abs(rotation) < 0.001f)
    {
        d3d9::rendering::fill_quad(device, min, max);
    }
    else
    {
        //matrix3x3f rotz = pt::rotation_z_matrix(D3DXToRadian(rotation));
        //vector2f translation;
        //translation[0] = min[0] + ((max[0] - min[0]) / 2);
        //translation[1] = min[1] + ((max[1] - min[1]) / 2);

        //float rot_piv_x = min[0] + (max[0] - min[0])/2;
        //float rot_piv_y = min[1] + (max[1] - min[1])/2;
        d3d9::rendering::fill_quad(device, min, max, vector2f(0.8f, -0.8f),rotation);
    }

    return result;
}

void draw_image::parse(config_node const& config,
    parameter_collection const& parameters)
{
    assert(config.key() == "draw_image");

    std::shared_ptr<parameter_node> curves
        = optional_child_node("curves", parameters);

    m_alpha = create_optional_curve_value<float>(config, "alpha",
        curves, 1.0f);
    m_intensity = create_optional_curve_value<float>(config, "intensity",
        curves, 1.0f);

    m_center = create_optional_curve_value<vector2f>(config, "center",
        curves, vector2f(0.5f, 0.5f));
    m_size = create_optional_curve_value<vector2f>(config, "size",
        curves, vector2f(1.0f, 1.0f));
    m_rotation = create_optional_curve_value<float>(config, "rotation",
        curves, 0.0f);

	m_ignore_alpha_channel = create_optional_curve_value<bool>(config, "ignore_alpha_channel",
		curves, false);
}

void draw_image::init(d3d9::graphics_device& device)
{
    try
    {
		bool is_alpha_channel_ignored = m_ignore_alpha_channel->value_at(0.0f);

		if (is_alpha_channel_ignored)
		{
			m_pixel_shader = d3d9::compile_new_pixel_shader(device,
				pixel_ignore_alpha_channel, "main_ps", "ps_3_0");
		}
		else
		{
			m_pixel_shader = d3d9::compile_new_pixel_shader(device,
				pixel, "main_ps", "ps_3_0");
		}

        m_vertex_shader = d3d9::compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(d3d9::compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("draw_image"));
        throw;
    }
}

}} // namespace content::sequences
