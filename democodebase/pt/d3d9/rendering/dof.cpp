#include "dof.h"
#include "post_processing.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/exceptions/string_context.h"
#include "pt/parameter_node_utility.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/path.h"
#include "pt/input_stream_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {

    texture_2d create_result_texture(graphics_device& device,
        vector2i resolution, D3DFORMAT format)
    {
        return texture_2d_builder(device)
            .resolution(resolution)
            .levels(1)
            .usage(D3DUSAGE_RENDERTARGET)
            .format(format)
            .pool(D3DPOOL_DEFAULT)
            .build();
    }

} // anonymous namespace

dof::dof(graphics_device& device,
    parameter_node const& parameter, vector2i resolution,
    D3DFORMAT format)
:   m_final_texture(create_result_texture(device, resolution, format))
{
    if (parameter.name() != "depth_of_field")
    {
        throw any_error("invalid parameter for depth_of_field \"",
            string_wrapper(parameter.name()), "\"");
    }

    m_focus = animatable_child<float>("focus", parameter);
    m_range = animatable_child<float>("range", parameter);
    m_blur_rgb_dist = animatable_child<float>("blur_rgb_dist", parameter);
    m_gain = animatable_child<float>("gain", parameter);
    m_threshold = animatable_child<float>("threshold", parameter);

	// Load HLSL
	std::string hlsl;
	{
		pt::path shader_path("data/shaders/hlsl/dof.hlsl");
		hlsl += (pt::format("#line 1 \"%1%\"\n") % shader_path.string()).move_string();
        hlsl += read_text_file(shader_path);
	}

    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
			hlsl, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            hlsl,//pass_through_vertex_shader_hlsl_with_texcoord(),
            "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("depth_of_field"));
        throw;
    }
}

void dof::update(float time)
{
    m_current_time = time;
}

void dof::draw(texture_2d const& source, texture_2d const& source2)
{
	std::shared_ptr<d3d9::texture_2d> depth_text = d3d9::default_texture_2d_collection().find("DepthTexture");

	set_texture(source, 0);
	set_texture(source2, 1);
	set_texture(*depth_text,2);
	

    m_pixel_shader->use();
    m_vertex_shader->use();
	
    graphics_device& device = source.mutable_device();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	set_sampler_state(device, 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	set_sampler_state(device, 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	set_sampler_state(device, 2, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	set_sampler_state(device, 2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	set_sampler_state(device, 2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    if (m_pixel_shader->constants().count() > 0)
    {
		
		scoped_render_target_texture rt(m_final_texture);

        float focus  = m_focus->value_at(m_current_time);
        float range  = m_range->value_at(m_current_time);
		float blur_rgb_dist = m_blur_rgb_dist->value_at(m_current_time);
		float gain   = m_gain->value_at(m_current_time);
		float threshold = m_threshold->value_at(m_current_time);

		vector2i res = source.resolution();
		float width = static_cast<float>(res[0]);
		float height = static_cast<float>(res[1]);
        
        constant const& c1 = m_pixel_shader->constants().by_name("focus");
        constant const& c2 = m_pixel_shader->constants().by_name("range");
		constant const& c3 = m_pixel_shader->constants().by_name("texel_width");
		constant const& c4 = m_pixel_shader->constants().by_name("texel_height");
		constant const& c5 = m_pixel_shader->constants().by_name("blur_rgb_dist");
		constant const& c6 = m_pixel_shader->constants().by_name("gain");
		constant const& c7 = m_pixel_shader->constants().by_name("threshold");

        set_constant(c1, m_pixel_shader->constants(), focus);
        set_constant(c2, m_pixel_shader->constants(), range);
		set_constant(c3, m_pixel_shader->constants(), 1.0f / width);
		set_constant(c4, m_pixel_shader->constants(), 1.0f / height);
		set_constant(c5, m_pixel_shader->constants(), blur_rgb_dist);
		set_constant(c6, m_pixel_shader->constants(), gain);
		set_constant(c7, m_pixel_shader->constants(), threshold);

		fill_fullscreen_quad(device);
    }
	
    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
	set_texture(device, 1, com_ptr<IDirect3DTexture9>());
	set_texture(device, 2, com_ptr<IDirect3DTexture9>());
}

texture_2d const& dof::result() const
{
    return m_final_texture;
}


}}} // namespace pt::d3d9::rendering
