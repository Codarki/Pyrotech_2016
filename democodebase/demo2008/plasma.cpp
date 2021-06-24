#include "plasma.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/exceptions/string_context.h"
#include "pt/path.h"

namespace content {

using namespace d3d9;

namespace {
    std::unique_ptr<d3d9::texture_2d> create_texture(graphics_device& device,
        vector2i resolution, D3DFORMAT format)
    {
        return texture_2d_builder(device)
            .resolution(resolution)
            .levels(1)
            .usage(D3DUSAGE_RENDERTARGET)
            .format(format)
            .pool(D3DPOOL_DEFAULT)
            .build_unique_ptr();
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
        "#line 42 " __FILE__ "\n"
        "float time_x;\n"
        "float time_y;\n"
        "float2 scale;\n"
        "float2 translation;\n"

        "texture2D source_texture;\n"
        "sampler2D s0;\n"

        "struct magnet\n"
        "{\n"
        "    float2 position;\n"
        "    float4 strength;\n"
        "};\n"
        "magnet magnets[8];\n"

        "float to_radian(float x)\n"
        "{\n"
        "    return x * 3.1415 * 2;\n"
        "}\n"
        "float2 to_radian(float2 x)\n"
        "{\n"
        "    return x * 3.1415 * 2;\n"
        "}\n"

        "#line 56\n"
        "float2 distort(float2 frequency, float2 scale)\n"
        "{\n"
        "    float2 r = to_radian(frequency);\n"
        "    return float2(sin(r.y), sin(r.x)) * scale;\n"
        "}\n"

        "#line 63\n"
        "float2 plasma(float2 pos, float2 scale, float scramble, float2 angle_offset)\n"
        "{\n"
        "    float2 y_axis_value;\n"
        "    float2 x_axis_value;\n"
        "    y_axis_value.x = cos(pos.y * scale.y * 0.67) * scramble + angle_offset.x;\n"
        "    y_axis_value.y = sin(pos.y * scale.y * 0.95) * scramble + angle_offset.y;\n"

        "    x_axis_value.x = cos(pos.x * scale.x * 0.78) * scramble;\n"
        "    x_axis_value.y = sin(pos.x * scale.x * 1.13) * scramble;\n"

        "    return y_axis_value + x_axis_value;\n"
        "}\n"

        "#line 77\n"
        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
        "    color = 0;\n"

        "#line 87\n"
        "    // Origo at center.\n"
        "    float2 centered_texcoord = texcoord - 0.5;\n"
        "    float2 pos = centered_texcoord * scale + translation;\n"
        "    float t = 1.0;\n"
        "    float t_coeff = 2.0;\n"
        "    float max_distort = 0.05;\n"

        "    float2 time = {time_x, time_y};\n"
        "    float2 time_coeff = {0.2, 0.2};\n"
        "    float2 time_coeff_delta = {0.3, 0.3};\n"

        "    float2 freq_offset_coeff = {0.4, 0.4};\n"
        "    float2 freq_offset_coeff_delta = {0.2, 0.2};\n"

        //"    float2 total_distort = {0.0, 0.0};\n"
        //"    for (int i = 0; i < 2; ++i)\n"
        //"    {\n"
        //"        float distort_factor = max_distort / (t);\n"
        //"        float2 current_time = time * (time_coeff + time_coeff_delta * i);\n"
        //"        float2 freq_offset_time = time * (freq_offset_coeff + freq_offset_coeff_delta * i);\n"

        //"        float2 frequency = texcoord * t;\n"
        //"        float2 freq_offset = {sin(freq_offset_time.y), sin(freq_offset_time.x)};\n"
        //"        frequency += freq_offset * 1.0;\n"

        //"        float2 strength;\n"
        //"        strength.x = sin(to_radian(current_time.x)) * distort_factor;\n"
        //"        strength.y = sin(to_radian(current_time.y)) * distort_factor;\n"
        //"        total_distort += distort(frequency, strength);\n"

        //"        t *= t_coeff;\n"
        //"    }\n"
        ////"    pos = total_distort;\n"
        //"    texcoord += total_distort;\n"

        //"    float scramble = 2;\n"
        //"    float2 angle_offset = {time_x, time_y};\n"
        //"    float2 pos2 = plasma(pos, scale, scramble, angle_offset * 5);\n"
        ////"    pos += float2(cos(pos2.x), sin(pos2.y));\n"
        //"    texcoord += float2(cos(pos2.x), sin(pos2.y)) * 0.1;\n"

        "    float radius = 0.2;\n"
        "    float strength = -0.8;\n"
        "    float smoothness = 3.0;\n"

        "    float2 total_magnet = {0.0, 0.0};\n"
        "    for (int i = 0; i < 8; ++i)\n"
        "    {\n"
        "        float2 vec = texcoord - magnets[i].position;\n"
        "        float2 dir = normalize(-vec);\n"
        "        float amount = saturate(1.0 - length(vec) / radius);\n"
        "        total_magnet += dir * length(vec) * pow(amount, smoothness) * strength;\n"
        "    }\n"
        "    texcoord += total_magnet;\n"

        //"    float2 jee = to_radian(total_magnet);\n"
        //"    float lilli = (cos(jee.x) + sin(jee.y)) / 2;\n"
        //"    color.xyz = lilli * 0.5 + 0.5;\n"

        "    color.xyz = tex2D(s0, texcoord);\n"

        "}\n"
        ;

        inline vector2f magnet_pos(float time, float x_coeff, float y_coeff)
        {
            return vector2f(
                std::cos(time * x_coeff) * 0.5f,
                std::sin(time * y_coeff) * 0.5f);
        }
} // anonymous namespace

plasma::plasma(d3d9::graphics_device& device)
: m_texture(create_texture(device, vector2i(512, 512), D3DFMT_A16B16G16R16F))
, m_background_texture(d3d9::load_texture_2d(device, "data/images/effects/testitunnel1.jpg"))
{
    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("plasma"));
        throw;
    }
}

void plasma::draw() const
{
    graphics_device& device = m_texture->mutable_device();

    m_pixel_shader->use();
    m_vertex_shader->use();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    {
        scoped_render_target_texture rt(*m_texture);

        constant_buffer const& constants = m_pixel_shader->constants();

        if (constant const* c = constants.try_get("time_x"))
            set_constant(*c, constants, m_time * 0.6f);

        if (constant const* c = constants.try_get("time_y"))
            set_constant(*c, constants, m_time * 0.9f);

        if (constant const* c = constants.try_get("scale"))
        {
            vector2f scale(2, 2);
            scale[0] += std::sin(m_time * 0.33f) * 0.001f;
            scale[1] += std::sin(m_time * 0.43f) * 0.001f;

            set_constant(*c, constants, scale);
        }

        if (constant const* c = constants.try_get("translation"))
        {
            vector2f translation(
                std::cos(m_time * 0.73f), std::sin(m_time * 1.21f));
            translation *= 0.0000005f;

            set_constant(*c, constants, translation);
        }

        if (constant const* tmp_magnets = constants.try_get("magnets"))
        {
            constant const& magnets = *tmp_magnets;
            constant const& pos0 = magnets[0].at(0).by_name("position");
            constant const& pos1 = magnets[1].at(0).by_name("position");
            constant const& pos2 = magnets[2].at(0).by_name("position");
            constant const& pos3 = magnets[3].at(0).by_name("position");
            constant const& pos4 = magnets[4].at(0).by_name("position");
            constant const& pos5 = magnets[5].at(0).by_name("position");
            constant const& pos6 = magnets[6].at(0).by_name("position");
            constant const& pos7 = magnets[7].at(0).by_name("position");

            vector2f center(0.5, 0.5);

            vector2f position0 = center + magnet_pos(m_time + 0.1f, 0.43f, 0.23f);
            vector2f position1 = center + magnet_pos(m_time + 0.4f, 0.73f, 0.43f);
            vector2f position2 = center + magnet_pos(m_time + 0.7f, 0.93f, 0.61f);
            vector2f position3 = center + magnet_pos(m_time + 1.1f, 0.53f, 0.36f);
            vector2f position4 = center + magnet_pos(m_time + 1.4f, 0.23f, 0.67f);
            vector2f position5 = center + magnet_pos(m_time + 2.7f, 0.33f, 0.78f);
            vector2f position6 = center + magnet_pos(m_time + 2.1f, 0.37f, 0.41f);
            vector2f position7 = center + magnet_pos(m_time + 2.4f, 0.67f, 0.56f);
            set_constant(pos0, constants, position0);
            set_constant(pos1, constants, position1);
            set_constant(pos2, constants, position2);
            set_constant(pos3, constants, position3);
            set_constant(pos4, constants, position4);
            set_constant(pos5, constants, position5);
            set_constant(pos6, constants, position6);
            set_constant(pos7, constants, position7);
        }

        set_texture(*m_background_texture, 0);

        set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


        rendering::fill_fullscreen_quad(device);
    }

    set_texture(*m_texture, 0);
    rendering::fill_fullscreen_quad(device);
}

void plasma::update(float time)
{
    m_time = time;
}

} // namespace content
