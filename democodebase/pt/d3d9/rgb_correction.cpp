#include "rgb_correction.h"
#include "post_processing.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/parameter_node_utility.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/path.h"
#include "pt/exceptions/string_context.h"
#include "pt/d3d9/texture_2d_collection.h"

namespace pt { namespace d3d9 { namespace rendering {

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
	    "float rand_x;\n"
	    "float rand_y;\n"
        "float noise_strength;\n"
        "float frequency;\n"
	    "texture2D source_texture;\n"
	    "texture2D noise_texture;\n"
	    "sampler2D s0;\n"
	    "sampler2D s1;\n"

	    "void main_ps(\n"
		"    float2 texcoord : TEXCOORD0,\n"
		"    out float4 color : COLOR0)\n"
	    "{\n"
        "    color.xyz = tex2D(s0,texcoord).xyz;\n"
		"    texcoord.x = texcoord.x+rand_x;\n"
		"    texcoord.y = texcoord.y+rand_y;\n"
		"    float3 noise_color = tex2D(s1, texcoord * frequency).xyz * noise_strength;\n"
		"    color.xyz += noise_color;\n"
        "    color.w = 1;\n"
        "}\n"
        ;
} // anonymous namespace

rgb_correction::rgb_correction(graphics_device& device,
    shared_ptr<parameter_node const> parameter)
{

    if (parameter->name() != "noise")
    {
        throw any_error(format("invalid parameter for noise '%1%'")
            % parameter->name());
    }

    m_red_corr   = get_animatable<float>(*get_child("red_corr", parameter));
    m_green_corr = get_animatable<float>(*get_child("green_corr", parameter));
    m_blue_corr = get_animatable<float>(*get_child("blue_corr", parameter));
    
    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("noise"));
        throw;
    }
}

void rgb_correction::update(float time)
{
    m_current_time = time;
}

void rgb_correction::draw(texture_2d const& source)
{
    set_texture(source, 0);
    
    m_pixel_shader->use();
    m_vertex_shader->use();

    graphics_device& device = source.mutable_device();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    //set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    //set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    if (m_pixel_shader->constants().count() > 0)
    {
        float red_coord    = m_red_corr->value_at(m_current_time);
        float green_coord  = m_green_corr->value_at(m_current_time);
        float blue_coord   = m_blue_corr->value_at(m_current_time);


        constant const& c1 = m_pixel_shader->constants().by_name("red_coord");
        constant const& c2 = m_pixel_shader->constants().by_name("green_coord");
        constant const& c3 = m_pixel_shader->constants().by_name("blue_coord");

        set_constant(c1, m_pixel_shader->constants(), red_coord);
        set_constant(c2, m_pixel_shader->constants(), green_coord);
        set_constant(c3, m_pixel_shader->constants(), blue_coord);
    }

    fill_fullscreen_quad(device);

    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
}

}}} // namespace pt::d3d9::rendering
