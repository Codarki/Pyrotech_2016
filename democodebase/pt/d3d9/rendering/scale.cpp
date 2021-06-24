#include "scale.h"
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
#include "pt/exceptions/string_context.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    static const int MAX_SAMPLES = 16;

    vector2i get_scaled_resolution(vector2i original_resolution,
        scale::amount amount)
    {
        int divider = 0;
        if (amount == scale::downscale_2)
            divider = 2;
        else if (amount == scale::downscale_4)
            divider = 4;

        assert(divider > 0);

        vector2i resolution = original_resolution / divider;
        assert(resolution[0] > 0);
        assert(resolution[1] > 0);

        return resolution;
    }

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

    void set_sample_offsets_4(vector2i resolution,
        std::vector<vector4f>& samples)
    {
        vector2f texel_size;
        texel_size[0] = 1.0f / static_cast<float>(resolution[0]);
        texel_size[1] = 1.0f / static_cast<float>(resolution[1]);

        // Sample from the 16 surrounding points. Since the center point will
        // be in the exact center of 16 texels, a 0.5f offset is needed to
        // specify a texel center.
        size_t index = 0;
        samples.resize(8);

        for (size_t y = 0; y < 4; ++y)
        {
            for (size_t x = 0; x < 4;)
            {
                samples[index][0] = (x - 1.5f) * texel_size[0];
                samples[index][1] = (y - 1.5f) * texel_size[1];
                ++x;

                samples[index][2] = (x - 1.5f) * texel_size[0];
                samples[index][3] = (y - 1.5f) * texel_size[1];
                ++x;
                //samples[index][0] = (x - 1.0f) * texel_size[0];
                //samples[index][1] = (y - 1.5f) * texel_size[1];
                //++x;

                //samples[index][2] = (x - 1.0f) * texel_size[0];
                //samples[index][3] = (y - 1.5f) * texel_size[1];
                //++x;

                ++index;
            }
        }
    }

    void set_sample_offsets_2(vector2i resolution,
        std::vector<vector4f>& samples)
    {
        vector2f texel_size;
        texel_size[0] = 1.0f / static_cast<float>(resolution[0]);
        texel_size[1] = 1.0f / static_cast<float>(resolution[1]);

        // Sample from the 4 surrounding points. Since the center point will
        // be in the exact center of 4 texels, a 0.5f offset is needed to
        // specify a texel center.
        size_t index = 0;
        samples.resize(2);

        for (size_t y = 0; y < 2; ++y)
        {
            samples[index][0] = (0 - 0.5f) * texel_size[0];
            samples[index][1] = (y - 0.5f) * texel_size[1];
            samples[index][2] = (1 - 0.5f) * texel_size[0];
            samples[index][3] = (y - 0.5f) * texel_size[1];

            //samples[index][0] = (0 - 0.0f) * texel_size[0];
            //samples[index][1] = (y - 0.0f) * texel_size[1];
            //samples[index][2] = (1 - 0.0f) * texel_size[0];
            //samples[index][3] = (y - 0.0f) * texel_size[1];

            ++index;
        }
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

    char pixel_4[] =
        "texture2D source_texture;\n"
        "sampler2D s0;\n"
        "float4 sample_offsets[8];\n"

        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
        "    float4 sample = 0.0f;\n"
        "    for (int i = 0; i < 8; ++i)\n"
        "    {\n"
        "        sample += tex2D(s0, texcoord + sample_offsets[i].xy);\n"
        "        sample += tex2D(s0, texcoord + sample_offsets[i].zw);\n"
        "    }\n"
        "    color = sample / 16.0;\n"
        "}\n"
        ;
    char pixel_2[] =
        "texture2D source_texture;\n"
        "sampler2D s0;\n"
        "float4 sample_offsets[2];\n"

        "void main_ps(\n"
        "    float2 texcoord : TEXCOORD0,\n"
        "    out float4 color : COLOR0)\n"
        "{\n"
        "    float4 sample = 0.0f;\n"
        "    for (int i = 0; i < 2; ++i)\n"
        "    {\n"
        "        sample += tex2D(s0, texcoord + sample_offsets[i].xy);\n"
        "        sample += tex2D(s0, texcoord + sample_offsets[i].zw);\n"
        "    }\n"
        "    color = sample / 4.0;\n"
        "}\n"
        ;
} // anonymous namespace

scale::scale(graphics_device& device, vector2i original_resolution,
    amount amount, D3DFORMAT format)
:   m_texture(create_result_texture(device,
        get_scaled_resolution(original_resolution, amount), format))
,   m_resolution(get_scaled_resolution(original_resolution, amount))
{
    char* pixel_source = 0;
    switch(amount)
    {
    case downscale_2:
        set_sample_offsets_2(original_resolution, m_samples);
        pixel_source = pixel_2;
        break;

    case downscale_4:
        set_sample_offsets_4(original_resolution, m_samples);
        pixel_source = pixel_4;
        break;
    }

    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            pixel_source, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("scale"));
        throw;
    }
}

void scale::execute(texture_2d const& source)
{
    graphics_device& device = source.mutable_device();

    set_texture(source, 0);

    m_pixel_shader->use();
    m_vertex_shader->use();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    //set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    //set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    constant const& c = m_pixel_shader->constants().by_name("sample_offsets");
    set_constant(c, m_pixel_shader->constants(), m_samples);

    scoped_render_target_texture rt(m_texture);

    fill_fullscreen_quad(device);

    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
}

texture_2d const& scale::result() const
{
    return m_texture;
}

vector2i scale::resolution() const
{
    return m_resolution;
}

}}} // namespace pt::d3d9::rendering
