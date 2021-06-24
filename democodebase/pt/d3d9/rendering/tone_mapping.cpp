#include "tone_mapping.h"
#include "post_processing.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/input_stream_utility.h"
#include "pt/parameter_node_utility.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/math_utils.h"
#include "pt/path.h"
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

tone_mapping::tone_mapping(graphics_device& device,
    parameter_node const& parameter, vector2i resolution,
    D3DFORMAT format)
:   m_final_texture(create_result_texture(device, resolution, format))
{
    if (parameter.name() != "tone_mapping")
    {
        throw any_error(pt::format("invalid parameter for tone mapping \"%1%\"")
            % parameter.name());
    }

    // Decent defaults:
    // adapted_luminance 0.36
    // middle_gray 0.18

    m_adapted_luminance
        = animatable_child<float>("adapted_luminance", parameter);
    m_middle_gray
        = animatable_child<float>("middle_gray", parameter);
    m_saturation
        = animatable_child<float>("saturation", parameter);
    m_white_point
        = animatable_child<float>("white_point", parameter);
    m_color_correction
        = animatable_child<vector3f>("color_correction", parameter);
    m_negation_color
        = animatable_child<vector3f>("negation_color", parameter);

    m_attribute1
        = animatable_child<vector4f>("attribute1", parameter);
    m_attribute2
        = animatable_child<vector4f>("attribute2", parameter);
    m_attribute3
        = animatable_child<vector4f>("attribute3", parameter);
    m_attribute4
        = animatable_child<vector4f>("attribute4", parameter);
    m_attribute5
        = animatable_child<vector4f>("attribute5", parameter);

    // Load HLSL
    std::string hlsl;
    {
        pt::path shader_path("data/shaders/hlsl/tone_mapping_functions.hlsl");
        hlsl += (pt::format("#line 1 \"%1%\"\n") % shader_path.string()).move_string();
        hlsl += read_text_file(shader_path);
    }

    {
        pt::path shader_path("data/shaders/hlsl/tone_mapping.hlsl");
        hlsl += (pt::format("#line 1 \"%1%\"\n") % shader_path.string()).move_string();
        hlsl += read_text_file(shader_path);
    }

    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            hlsl, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            hlsl, "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("tone_mapping"));
        throw;
    }
}

void tone_mapping::update(float time)
{
    m_current_time = time;
}

void tone_mapping::draw(texture_2d const& source, texture_2d const& source2)
{
    set_texture(source, 0);
    set_texture(source2, 1);

    m_pixel_shader->use();
    m_vertex_shader->use();

    graphics_device& device = source.mutable_device();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    set_sampler_state(device, 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    //set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    //set_sampler_state(device, 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    set_sampler_state(device, 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    set_sampler_state(device, 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


    if (m_pixel_shader->constants().count() > 0)
    {
        float middle_gray = m_middle_gray->value_at(m_current_time);
        float adapted_luminance = m_adapted_luminance->value_at(m_current_time);
        float saturation = m_saturation->value_at(m_current_time);
        float white_point = m_white_point->value_at(m_current_time);
        vector3f color_correction = m_color_correction->value_at(m_current_time);
        vector3f negation_color = m_negation_color->value_at(m_current_time);

        if (middle_gray < 0.0f)
            middle_gray = 0.0f;

        if (adapted_luminance < 0.0f)
            adapted_luminance = 0.0f;

        if (white_point < 0.0f)
            white_point = 0.0f;

        saturation = pt::clamp(saturation, 0.0f, 1.0f);

        scoped_render_target_texture rt(m_final_texture);

        constant const& c1 = m_pixel_shader->constants().by_name("middle_gray");
        constant const& c2 = m_pixel_shader->constants().by_name("adapted_luminance");
        constant const& c3 = m_pixel_shader->constants().by_name("white_point");
        constant const& c4 = m_pixel_shader->constants().by_name("color_correction");
        constant const& c5 = m_pixel_shader->constants().by_name("saturation");
        constant const& c6 = m_pixel_shader->constants().by_name("negation_color");

        constant const* ca1 = m_pixel_shader->constants().try_get("attribute1");
        constant const* ca2 = m_pixel_shader->constants().try_get("attribute2");
        constant const* ca3 = m_pixel_shader->constants().try_get("attribute3");
        constant const* ca4 = m_pixel_shader->constants().try_get("attribute4");
        constant const* ca5 = m_pixel_shader->constants().try_get("attribute5");

        if (ca1)
        {
            vector4f attribute1 = m_attribute1->value_at(m_current_time);
            set_constant(*ca1, m_pixel_shader->constants(), attribute1);
        }

        if (ca2)
        {
            vector4f attribute2 = m_attribute2->value_at(m_current_time);
            set_constant(*ca2, m_pixel_shader->constants(), attribute2);
        }

        if (ca3)
        {
            vector4f attribute3 = m_attribute3->value_at(m_current_time);
            set_constant(*ca3, m_pixel_shader->constants(), attribute3);
        }

        if (ca4)
        {
            vector4f attribute4 = m_attribute4->value_at(m_current_time);
            set_constant(*ca4, m_pixel_shader->constants(), attribute4);
        }

        if (ca5)
        {
            vector4f attribute5 = m_attribute5->value_at(m_current_time);
            set_constant(*ca5, m_pixel_shader->constants(), attribute5);
        }

        set_constant(c1, m_pixel_shader->constants(), middle_gray);
        set_constant(c2, m_pixel_shader->constants(), adapted_luminance);
        set_constant(c3, m_pixel_shader->constants(), white_point);
        set_constant(c4, m_pixel_shader->constants(), color_correction);
        set_constant(c5, m_pixel_shader->constants(), saturation);
        set_constant(c6, m_pixel_shader->constants(), negation_color);

        fill_fullscreen_quad(device);
    }

   
    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
    set_texture(device, 1, com_ptr<IDirect3DTexture9>());
}

texture_2d const& tone_mapping::result() const
{
    return m_final_texture;
}

}}} // namespace pt::d3d9::rendering
