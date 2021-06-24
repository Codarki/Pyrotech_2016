#include "blur.h"
#include "post_processing.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/format.h"
#include "pt/config_node.h"
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

    /// Get the texture coordinate offsets to be used inside the gaussian
    /// blur 5x5
    void set_sample_offsets(vector2i resolution,
        std::vector<vector4f>& offsets, std::vector<vector4f>& weights,
        float multiplier)
    {
        vector2f texel_size;
        texel_size[0] = 1.0f / static_cast<float>(resolution[0]);
        texel_size[1] = 1.0f / static_cast<float>(resolution[1]);

        vector4f white(1, 1, 1, 1);
        float total_weight = 0.0f;

        size_t index = 0;
        offsets.resize(13);
        weights.resize(13);

        for (int y = -2; y < 3; ++y)
        {
            for (int x = -2; x < 3; ++x)
            {
                // Exclude pixels with a block distance greater than 2. This
                // will create a kernel which approximates a 5x5 kernel using
                // only 13 sample points instead of 25; this is necessary
                // since 2.0 shaders only support 16 texture grabs.
                if (std::abs(x) + std::abs(y) > 2)
                    continue;

                float distribution = gaussian_distribution(
                    static_cast<float>(x), static_cast<float>(y), 1.0f);

                offsets[index] = vector2f(x * texel_size[0], y * texel_size[1]);
                weights[index] = white * distribution;
                total_weight += weights[index][0];

                index++;
            }
        }
        assert(index == 13);

        // Divide the current weight by the total weight of all the samples;
        // Gaussian blur kernels add to 1.0f to ensure that the intensity of
        // the image isn't changed when the blur occurs. An optional
        // multiplier variable is used to add or remove image intensity
        // during the blur.
        for (size_t i = 0; i < index; ++i)
        {
            weights[i] /= total_weight;
            weights[i] *= multiplier;
        }
    }

    char pixel[] =
        "texture2D source_texture;\n"
        "texture2D strength_texture;\n"
        "sampler2D s0;\n"
        "sampler2D s1;\n"
        "float4 offsets[13];\n"
        "float4 weights[13];\n"

        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
        "    color = 0;\n"
        "    float4 strength = tex2D(s1, texcoord);\n"
        "    for (int i = 0; i < 13; ++i)\n"
        "    {\n"
        "        float2 adjusted_texcoord = texcoord + (offsets[i].xy);\n"
		"        color += weights[i] * tex2D(s0, adjusted_texcoord);\n"
        "    }\n"
        "}\n"
        ;
} // anonymous namespace

blur::blur(graphics_device& device, vector2i resolution, D3DFORMAT format,
    std::shared_ptr<animatable_value<std::shared_ptr<texture_2d>> const> blur_strength_texture)
: m_texture(create_result_texture(device, resolution, format))
, m_blur_strength_texture(blur_strength_texture)
, m_current_time(0.0f)
{
    set_sample_offsets(resolution, m_offsets, m_weights, 1.0f);

    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            pass_through_vertex_shader_hlsl_with_texcoord(),
            "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("blur"));
        throw;
    }
}

void blur::update(float time)
{
    m_current_time = time;
}

void blur::draw(texture_2d const& source)
{
    graphics_device& device = source.mutable_device();

    std::shared_ptr<texture_2d> blur_strength_texture = m_blur_strength_texture->value_at(m_current_time);

    // input textures
    set_texture(source, 0);
    set_texture(*blur_strength_texture, 1);

    scoped_render_target_texture rt(m_texture);

    m_pixel_shader->use();
    m_vertex_shader->use();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    set_sampler_state(device, 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    constant const& c0 = m_pixel_shader->constants().by_name("offsets");
    constant const& c1 = m_pixel_shader->constants().by_name("weights");
    set_constant(c0, m_pixel_shader->constants(), m_offsets);
    set_constant(c1, m_pixel_shader->constants(), m_weights);

    fill_fullscreen_quad(device);

    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
    set_texture(device, 1, com_ptr<IDirect3DTexture9>());
}

texture_2d const& blur::result() const
{
    return m_texture;
}

vector2i blur::resolution() const
{
    return m_texture.resolution();
}

}}} // namespace pt::d3d9::rendering
