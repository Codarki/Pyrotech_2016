#include "noise.h"
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

noise::noise(graphics_device& device,
    parameter_node const& parameter)
{
    if (parameter.name() != "noise")
    {
        throw any_error(format("invalid parameter for noise '%1%'")
            % parameter.name());
    }

    // Decent defaults:
    // noise_strength 0.2
    // noise_speed
    m_noise_strength = animatable_child<float>("strength", parameter);
    m_noise_speed = animatable_child<float>("speed", parameter);
    m_noise_frequency = animatable_child<float>("frequency", parameter);
    
    //Load noise texture
    std::string image_name = "data/images/noise.jpg";
    
    m_noise_texture = d3d9::default_texture_2d_collection().find(image_name);
    if (!m_noise_texture)
    {
        m_noise_texture = d3d9::load_texture_2d(device, image_name);
        d3d9::default_texture_2d_collection().add(image_name, m_noise_texture);
    }
    assert(m_noise_texture);

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

void noise::update(float time)
{
    m_current_time = time;
}

void noise::draw(texture_2d const& source)
{
    set_texture(source, 0);
    set_texture(*m_noise_texture, 1);
    
    m_pixel_shader->use();
    m_vertex_shader->use();

    graphics_device& device = source.mutable_device();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    //set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    //set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    set_sampler_state(device, 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    set_sampler_state(device, 1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    set_sampler_state(device, 1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    if (m_pixel_shader->constants().count() > 0)
    {
        float noise_strength = m_noise_strength->value_at(m_current_time);
        float noise_speed    = m_noise_speed->value_at(m_current_time);
        float noise_frequency = m_noise_frequency->value_at(m_current_time);

        if (noise_strength < 0.0f)
            noise_strength = 0.0f;
        if (noise_speed < 0.0f)
            noise_speed = 0.0f;

        float rand_x,rand_y;
        int lowest=0, highest=40000;
        int range=(highest-lowest)+1;

        rand_x = static_cast<float>((lowest+int(range*rand()/(RAND_MAX + 1.0))) / 40000.0);        
        rand_y = static_cast<float>((lowest+int(range*rand()/(RAND_MAX + 1.0))) / 40000.0);
        
        rand_x = rand_x * noise_speed;
        rand_y = rand_y * noise_speed;
        

        constant const& c1 = m_pixel_shader->constants().by_name("noise_strength");
        constant const& c2 = m_pixel_shader->constants().by_name("rand_x");
        constant const& c3 = m_pixel_shader->constants().by_name("rand_y");
        constant const& c4 = m_pixel_shader->constants().by_name("frequency");

        set_constant(c1, m_pixel_shader->constants(), noise_strength);
        set_constant(c2, m_pixel_shader->constants(), rand_x);
        set_constant(c3, m_pixel_shader->constants(), rand_y);
        set_constant(c4, m_pixel_shader->constants(), noise_frequency);
    }

    fill_fullscreen_quad(device);

    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
    set_texture(device, 1, com_ptr<IDirect3DTexture9>());
}

}}} // namespace pt::d3d9::rendering
