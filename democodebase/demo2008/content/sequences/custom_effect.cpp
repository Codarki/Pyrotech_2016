#include "custom_effect.h"
#include "rendertarget.h"
#include "../utility.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/graphics_device.h"
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
#include "pt/d3d9/utility.h"


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
		"float time;\n"
		"float speed_rot;\n"
		"float speed_zoom;\n"
        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
		"    float2 p5 = float2(.5, .5);\n" // center point
		"    float2 offset_tc = texcoord - p5;\n" //re-center texture coordinate
		"    float dis = length(offset_tc);\n" // distance to the center
		"    float rotation = speed_rot + sin(time);\n"
		"    float zoomamount = pow(dis,speed_zoom+sin(time));\n" // * sin(time) / time;
		"    float angle = atan2(offset_tc.y, offset_tc.x) + rotation*(dis-1);\n" // new angle
		"    float2 new_tc = zoomamount *dis*.7 *float2(cos(angle), sin(angle)) + p5;\n" // new texture coordinate 
        "    float4 original_color = tex2D(source_sampler, new_tc);\n"
        "    color = pow((abs(original_color) + 0.055)/ 1.055, 2.4);\n"
		//"    color = original_color;\n"
        "    color.xyz *= intensity;\n"
        "    color.w *= alpha;\n"
        "}\n";
} // anonymous namespace

custom_effect::custom_effect(d3d9::graphics_device& device,
    config_node const& config, parameter_collection const& parameters)
: simple_composite_sequence(config)
{
    parse(config, parameters);
    init(device);
}

custom_effect::custom_effect(d3d9::graphics_device& device,
    std::shared_ptr<composite_sequence> input, config_node const& config,
    parameter_collection const& parameters)
: simple_composite_sequence(config)
{
    parse(config, parameters);
    add_child(input);
    init(device);
}

std::shared_ptr<d3d9::texture_2d> custom_effect::on_draw(rendertarget& target,
    float time, std::shared_ptr<d3d9::texture_2d> child_result)
{
    float alpha = m_alpha->value_at(time);
    if (alpha < 0.0f)
        return child_result;

    assert(child_result);

    // Draw image to \a target.
    scoped_optional_rendertarget scoped_target(target);

    d3d9::graphics_device& device = child_result->device();

    m_pixel_shader->use();
    m_vertex_shader->use();

    d3d9::set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    //d3d9::set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    //d3d9::set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    if (m_pixel_shader->constants().count() > 0)
    {
        float intensity = m_intensity->value_at(time);

        alpha = clamp(alpha, 0.0f, 1.0f);
        intensity = std::max(intensity, 0.0f);

        d3d9::constant const& c1 = m_pixel_shader->constants().by_name("alpha");
        d3d9::constant const& c2 = m_pixel_shader->constants().by_name("intensity");

		d3d9::constant const& c3 = m_pixel_shader->constants().by_name("speed_rot");
		d3d9::constant const& c4 = m_pixel_shader->constants().by_name("speed_zoom");
		d3d9::constant const& c5 = m_pixel_shader->constants().by_name("time");

        d3d9::set_constant(c1, m_pixel_shader->constants(), alpha);
        d3d9::set_constant(c2, m_pixel_shader->constants(), intensity);
        d3d9::set_constant(c3, m_pixel_shader->constants(), m_speed_rot);
		d3d9::set_constant(c4, m_pixel_shader->constants(), m_speed_zoom);
		d3d9::set_constant(c5, m_pixel_shader->constants(), time);
    }

    d3d9::scoped_render_state alpha_state(device, D3DRS_ALPHABLENDENABLE, TRUE);
	d3d9::scoped_render_state erkki(device,D3DRS_BLENDOP,D3DBLENDOP_ADD);

    //d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_ONE);

	//d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_ONE);


    d3d9::set_texture(*child_result, 0);
    //d3d9::rendering::fill_fullscreen_quad(device);

    vector2f center = m_center->value_at(time);
    vector2f size = m_size->value_at(time);

    vector2f min = center - (size * 0.5f);
    vector2f max = min + size;
	
    d3d9::rendering::fill_quad(device, min, max);
   
	//LPD3DXMESH Sphere;
	//D3DXCreateSphere(device.native().get(),10,10,10,&Sphere,NULL);
	//Sphere->DrawSubset(0);

    return target.texture();
}

void custom_effect::parse(config_node const& config,
    parameter_collection const& parameters)
{
    assert(config.key() == "custom_effect");
	config.parse_optional_child("speed_rotate", m_speed_rot);
	config.parse_optional_child("speed_zoom", m_speed_zoom);

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
}

void custom_effect::init(d3d9::graphics_device& device)
{
    try
    {
        m_pixel_shader = d3d9::compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = d3d9::compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(d3d9::compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("custom_effect"));
        throw;
    }
}

}} // namespace content::sequences
