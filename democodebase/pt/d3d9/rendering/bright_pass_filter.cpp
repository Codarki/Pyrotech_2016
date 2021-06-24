#include "bright_pass_filter.h"
#include "post_processing.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/input_stream_utility.h"
#include "pt/any_error.h"
#include "pt/parameter_node_utility.h"
#include "pt/format.h"
#include "pt/path.h"
#include "pt/string_wrapper.h"
#include "pt/exceptions/string_context.h"

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

using std::move;

bright_pass_filter::bright_pass_filter(
    std::shared_ptr<pixel_shader>&& pixel_shader,
    std::shared_ptr<vertex_shader>&& vertex_shader,
    texture_2d&& texture,
    animatable_value<float>&& adapted_luminance,
    animatable_value<float>&& middle_gray,
    animatable_value<float>&& threshold,
    animatable_value<float>&& offset,
    animatable_value<float>&& white_point)
: m_pixel_shader(move(pixel_shader))
, m_vertex_shader(move(vertex_shader))
, m_texture(move(texture))
, m_current_time(0.0f)
, m_adapted_luminance(move(adapted_luminance))
, m_middle_gray(move(middle_gray))
, m_threshold(move(threshold))
, m_offset(move(offset))
, m_white_point(move(white_point))
{
}

void bright_pass_filter::update(float time)
{
    m_current_time = time;
}

void bright_pass_filter::draw(texture_2d const& source)
{
    set_texture(source, 0);

    m_pixel_shader->use();
    m_vertex_shader->use();

    if (m_pixel_shader->constants().count() > 0)
    {
        float middle_gray = m_middle_gray.value_at(m_current_time);
        float adapted_luminance = m_adapted_luminance.value_at(m_current_time);
        float threshold = m_threshold.value_at(m_current_time);
        float offset = m_offset.value_at(m_current_time);
        float white_point = m_white_point.value_at(m_current_time);

        if (middle_gray < 0.0f)
            middle_gray = 0.0f;

        if (adapted_luminance < 0.0f)
            adapted_luminance = 0.0f;

        constant const& c1 = m_pixel_shader->constants().by_name("middle_gray");
        constant const& c2 = m_pixel_shader->constants().by_name("adapted_luminance");
        constant const& c3 = m_pixel_shader->constants().by_name("threshold");
        constant const& c4 = m_pixel_shader->constants().by_name("offset");
        constant const& c5 = m_pixel_shader->constants().by_name("white_point");
        set_constant(c1, m_pixel_shader->constants(), middle_gray);
        set_constant(c2, m_pixel_shader->constants(), adapted_luminance);
        set_constant(c3, m_pixel_shader->constants(), threshold);
        set_constant(c4, m_pixel_shader->constants(), offset);
        set_constant(c5, m_pixel_shader->constants(), white_point);
    }

    graphics_device& device = source.mutable_device();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    scoped_render_target_texture rt(m_texture);

    fill_fullscreen_quad(device);

    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
}

texture_2d const& bright_pass_filter::result() const
{
    return m_texture;
}

// free functions

std::unique_ptr<bright_pass_filter> build_bright_pass_filter(graphics_device& device,
    parameter_node const& parameter, vector2i resolution,
    D3DFORMAT format)
{
    if (parameter.name() != "bright_pass")
    {
        throw any_error("invalid parameter for bright pass \"",
            string_wrapper(parameter.name()), "\"");
    }

    texture_2d texture(create_result_texture(device, resolution, format));

    // Decent defaults:
    // threshold 0.20
    // offset 5.00

    animatable_value<float> adapted_luminance = *animatable_child<float>("adapted_luminance", parameter);
    animatable_value<float> middle_gray = *animatable_child<float>("middle_gray", parameter);
    animatable_value<float> threshold = *animatable_child<float>("threshold", parameter);
    animatable_value<float> offset = *animatable_child<float>("offset", parameter);
    animatable_value<float> white_point = *animatable_child<float>("white_point", parameter);

    // Load HLSL
    std::string hlsl;
    {
        pt::path shader_path("data/shaders/hlsl/tone_mapping_functions.hlsl");
        hlsl += (pt::format("#line 1 \"%1%\"\n") % shader_path.string()).move_string();
        hlsl += read_text_file(shader_path);
    }
    {
        pt::path shader_path("data/shaders/hlsl/bright_pass.hlsl");
        hlsl += (pt::format("#line 1 \"%1%\"\n") % shader_path.string()).move_string();
        hlsl += read_text_file(shader_path);
    }

    std::shared_ptr<pixel_shader> pixel_shader;
    std::shared_ptr<vertex_shader> vertex_shader;
    try
    {
        pixel_shader = compile_new_pixel_shader(device,
            hlsl, "main_ps", "ps_3_0");

        vertex_shader = compile_new_vertex_shader(device,
            hlsl, "main_vs", "vs_3_0");
    }
    catch (compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("bright_pass_filter"));
        throw;
    }

    //return std::unique_ptr<bright_pass_filter>(new bright_pass_filter(
    //    move(pixel_shader),
    //    move(vertex_shader),
    //    move(texture),
    //    move(adapted_luminance),
    //    move(middle_gray),
    //    move(threshold),
    //    move(offset),
    //    move(white_point)
    //    ));

    return std::unique_ptr<bright_pass_filter>(new bright_pass_filter(
        static_cast<std::shared_ptr<d3d9::pixel_shader>&&>(pixel_shader),
        static_cast<std::shared_ptr<d3d9::vertex_shader>&&>(vertex_shader),
        static_cast<texture_2d&&>(texture),
        static_cast<animatable_value<float>&&>(adapted_luminance),
        static_cast<animatable_value<float>&&>(middle_gray),
        static_cast<animatable_value<float>&&>(threshold),
        static_cast<animatable_value<float>&&>(offset),
        static_cast<animatable_value<float>&&>(white_point)
        ));
}

}}} // namespace pt::d3d9::rendering
