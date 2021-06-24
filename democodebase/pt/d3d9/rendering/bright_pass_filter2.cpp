#include "bright_pass_filter2.h"
#include "post_processing.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/parameter_node_utility.h"
#include "pt/boost/format.h"
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

        "static float luminance = 0.08f;\n"
        "static const float middle_gray = 0.18f;\n"
        "static const float white_cutoff = 0.8f;\n"

        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
        "    color = tex2D(source_sampler, texcoord);\n"
        "    float t = saturate(pow(length(color) / 3.0, 10));\n"
        "    color = color * t;\n"
        //"    color = color * 0.55;\n"
        "    color = color / (5.0f + color);\n"

        //"    color = tex2D(source_sampler, texcoord);\n"
        //"    color *= middle_gray / ( luminance + 0.001f );\n"
        //"    color *= ( 1.0f + ( color / ( white_cutoff * white_cutoff ) ) );\n"
        //"    color -= 5.0f;\n"
        //"    color = max( color, 0.0f );\n"
        //"    color /= ( 10.0f + color );\n"

        "    color.w = 1;\n"
        "}\n"
        ;
} // anonymous namespace

bright_pass_filter2::bright_pass_filter2(graphics_device& device,
    parameter_node const& , vector2i resolution,
    D3DFORMAT format)
:   m_texture(create_result_texture(device, resolution, format))
{
    // Decent defaults:
    // threshold 0.20
    // offset 5.00

    //m_adapted_luminance = get_animatable<float>(
    //    *optional_child_node("adapted_luminance", parameters));

    //m_middle_gray = get_animatable<float>(
    //    *optional_child_node("middle_gray", parameters));

    //m_threshold = get_animatable<float>(
    //    *optional_child_node("threshold", parameters));

    //m_offset = get_animatable<float>(
    //    *optional_child_node("offset", parameters));

    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("bright_pass_filter2"));
        throw;
    }
}

void bright_pass_filter2::update(float time)
{
    m_current_time = time;
}

void bright_pass_filter2::draw(texture_2d const& source)
{
    set_texture(source, 0);

    m_pixel_shader->use();
    m_vertex_shader->use();

    graphics_device& device = source.mutable_device();

    //if (m_pixel_shader->constants().count() > 0)
    //{
    //    float middle_gray = m_middle_gray->value_at(m_current_time);
    //    float adapted_luminance = m_adapted_luminance->value_at(m_current_time);
    //    float threshold = m_threshold->value_at(m_current_time);
    //    float offset = m_offset->value_at(m_current_time);

    //    constant const& c1 = m_pixel_shader->constants().by_name("middle_gray");
    //    constant const& c2 = m_pixel_shader->constants().by_name("adapted_luminance");
    //    constant const& c3 = m_pixel_shader->constants().by_name("threshold");
    //    constant const& c4 = m_pixel_shader->constants().by_name("offset");
    //    set_constant(m_pixel_shader->constants(), c1.handle(), middle_gray);
    //    set_constant(m_pixel_shader->constants(), c2.handle(), adapted_luminance);
    //    set_constant(m_pixel_shader->constants(), c3.handle(), threshold);
    //    set_constant(m_pixel_shader->constants(), c4.handle(), offset);
    //}

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    scoped_render_target_texture rt(m_texture);

    fill_fullscreen_quad(device);

    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
}

texture_2d const& bright_pass_filter2::result() const
{
    return m_texture;
}

}}} // namespace pt::d3d9::rendering
