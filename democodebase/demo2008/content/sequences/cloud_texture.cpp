#include "cloud_texture.h"
#include "basic_composite_sequence.h"
#include "draw_to_texture.h"
#include "rendertarget.h"
#include "../utility.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/exceptions/string_context.h"
#include "pt/config_node.h"
#include "pt/file_input_stream.h"
#include "pt/data_stream_content.h"
#include "pt/parameter_collection.h"
#include "pt/parameter_node.h"
#include "pt/format.h"
#include "pt/std/cassert.h"

namespace content { namespace sequences {

namespace {
} // anonymous namespace

class cloud_texture::impl
{
public:
    impl(config_node const& config,
        parameter_collection const& parameters)
    : m_basic(config, parameters)
    {
        assert(config.key() == "cloud_texture");

        m_target_texture = find_shared_texture(config, parameters, "target_shared_texture");

        std::shared_ptr<parameter_node> curves = optional_child_node("curves", parameters);
        m_cutoff_min = create_optional_curve_value<vector4f>(config, "cutoff_min", curves, vector4f(0.3f, 0.3f, 0.3f, 0.0f));
        m_cutoff_max = create_optional_curve_value<vector4f>(config, "cutoff_max", curves, vector4f(0.5f, 0.5f, 0.5f, 1.0f));
        m_speed = create_optional_curve_value<vector2f>(config, "speed", curves, vector2f(0.15f, 0.15f));
        m_time = create_optional_curve_value<float>(config, "time", curves, 0.0f);

        std::string hlsl;

        {
            std::string shader_filename;
            config.parse_child("shader", shader_filename);

            pt::path shader_path(shader_filename);
            file_input_stream file_stream(shader_path);
            data_stream_content data(file_stream);

            char const* ptr = reinterpret_cast<char const*>(data.data());
            std::string shader_file_text(ptr, data.size());
            assert(shader_file_text.c_str()[data.size()] == 0);

            hlsl = (format("#line 1 \"%1%\"\n") % shader_filename).move_string();
            hlsl += shader_file_text;
        }

        try
        {
            d3d9::graphics_device& device = m_target_texture->device();

            m_pixel_shader = d3d9::compile_new_pixel_shader(device,
                hlsl, "main_ps", "ps_3_0");

            m_vertex_shader = d3d9::compile_new_vertex_shader(device,
                hlsl, "main_vs", "vs_3_0");
        }
        catch(d3d9::compilation_error& e)
        {
            e.attach_context(new exceptions::string_context("cloud_texture"));
            throw;
        }
    }

    basic_composite_sequence const& basic() const
    {
        return m_basic;
    }

    basic_composite_sequence& basic()
    {
        return m_basic;
    }

    void draw(float time)
    {
        d3d9::graphics_device& device = m_target_texture->device();

        // Get input texture and set render target.
        rendertarget no_target_for_input;
		std::shared_ptr<d3d9::texture_2d> input_texture = m_basic.draw(no_target_for_input, time);

        d3d9::scoped_render_target_texture scoped_target(*m_target_texture);

        // Clear.
        DWORD flags = D3DCLEAR_TARGET;
        vector4f clear_color(1, 1, 1, 1);
        d3d9::clear(device, flags, clear_color, 1.0f, 0);

        // Draw.
        m_pixel_shader->use();
        m_vertex_shader->use();

        d3d9::set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(device, 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

        d3d9::set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        d3d9::set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        d3d9::set_sampler_state(device, 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);


        if (m_pixel_shader->constants().count() > 0)
        {
            if (d3d9::constant const* cutoff_min_constant = m_pixel_shader->constants().try_get("cutoff_min"))
            {
                const vector4f cutoff_min = m_cutoff_min->value_at(time);
                d3d9::set_constant<float,4>(*cutoff_min_constant, m_pixel_shader->constants(), cutoff_min);
            }

            if (d3d9::constant const* cutoff_max_constant = m_pixel_shader->constants().try_get("cutoff_max"))
            {
                const vector4f cutoff_max = m_cutoff_max->value_at(time);
                d3d9::set_constant<float,4>(*cutoff_max_constant, m_pixel_shader->constants(), cutoff_max);
            }

            if (d3d9::constant const* speed_constant = m_pixel_shader->constants().try_get("speed"))
            {
                const vector2f speed = m_speed->value_at(time);
                d3d9::set_constant<float,2>(*speed_constant, m_pixel_shader->constants(), speed);
            }

            float real_time = time;
            if (!m_time->is_constant())
                real_time = m_time->value_at(time);

            d3d9::constant const& c1 = m_pixel_shader->constants().by_name("time");
            d3d9::set_constant(c1, m_pixel_shader->constants(), real_time);
        }

        d3d9::scoped_render_state alpha_state(device, D3DRS_ALPHABLENDENABLE, FALSE);
        d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
        d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

        //d3d9::scoped_render_state alpha_state(device, D3DRS_ALPHABLENDENABLE, TRUE);
        //d3d9::scoped_render_state src_blend(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        //d3d9::scoped_render_state dst_blend(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        d3d9::set_texture(*input_texture, 0);
        d3d9::rendering::fill_fullscreen_quad(device);
    }

    std::shared_ptr<d3d9::texture_2d> m_target_texture;
    basic_composite_sequence m_basic;
    std::shared_ptr<animatable_value<vector4f> const > m_cutoff_min;
    std::shared_ptr<animatable_value<vector4f> const > m_cutoff_max;
    std::shared_ptr<animatable_value<vector2f> const > m_speed;
    std::shared_ptr<animatable_value<float> const > m_time;

    std::shared_ptr<d3d9::pixel_shader> m_pixel_shader;
    std::shared_ptr<d3d9::vertex_shader> m_vertex_shader;
};

// cloud_texture

cloud_texture::cloud_texture(
    config_node const& config, parameter_collection const& parameters)
: m_impl(new impl(config, parameters))
{
}

string_wrapper const& cloud_texture::name() const
{
    return m_impl->basic().name();
}

std::shared_ptr<d3d9::texture_2d> cloud_texture::draw(rendertarget&, float time)
{
    m_impl->draw(time);
    return nullptr;
}

float cloud_texture::start_time() const
{
    return m_impl->basic().start_time();
}

float cloud_texture::end_time() const
{
    return m_impl->basic().end_time();
}

void cloud_texture::set_override_view(d3d9::rendering::item* view)
{
    return m_impl->basic().set_override_view(view);
}

void cloud_texture::link_inputs(composite_sequence_collection const& sequences)
{
    return m_impl->basic().link_inputs(sequences);
}

}} // namespace content::sequences
