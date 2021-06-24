#include "bloom.h"
#include "post_processing.h"
#include "blur.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/any_error.h"
#include "pt/string_wrapper.h"
#include "pt/parameter_node_utility.h"
#include "pt/exceptions/string_context.h"


#include "pt/d3d9/texture_2d_data_set_builder.h"
#include "pt/d3d9/texture_2d_source.h"
#include "pt/math/step_curve.h"


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

    void set_sample_offsets(int texels, std::vector<float>& offsets,
        std::vector<vector4f>& weights, float deviation, float multiplier)
    {
        float texel_size = 1.0f / static_cast<float>(texels);

        size_t side_len = 7;
        size_t samples = 1 + 2 * side_len;

        weights.resize(samples);
        offsets.resize(samples);

        // Fill the center texel
        float weight = multiplier * gaussian_distribution(0, 0, deviation);
        weights[0] = vector4f(weight, weight, weight, 1.0f);
        offsets[0] = 0.0f;

        // Fill the first half
        for (size_t i = 1; i < side_len + 1; ++i)
        {
            // Get the Gaussian intensity for this offset
            weight = multiplier * gaussian_distribution(
                static_cast<float>(i), 0, deviation);

            offsets[i] = i * texel_size;
            weights[i] = vector4f(weight, weight, weight, 1.0f);
        }

        // Mirror to the second half
        for (size_t i = side_len + 1; i < samples; ++i)
        {
            offsets[i] = -offsets[i - side_len];
            weights[i] = weights[i - side_len];
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

	char pixel[] =
		"texture2D source_texture;\n"
		"sampler2D s0;\n"
		"texture2D horizontal_strength_texture;\n"
		"sampler2D s1;\n"
		"float4 offsets[15];\n"
		"float4 weights[15];\n"

		"void main_ps(\n"
		"    float2 texcoord : TEXCOORD0,\n"
		"    out float4 color : COLOR0)\n"
		"{\n"
		"    color = 0;\n"
		"    for (int i = 0; i < 15; ++i)\n"
		"    {\n"
		"        color += weights[i] * tex2D(s0, texcoord + offsets[i].xy);\n"
		"    }\n"
		//"    color *= tex2D(s1, texcoord);\n"
        //"    float t = saturate(pow(length(color) / 20.0, 2));\n"
        //"    color = color * t;\n"
        "}\n"
        ;
} // anonymous namespace

bloom::bloom(graphics_device& device,
    parameter_node const& parameter, vector2i resolution,
    D3DFORMAT format)
: m_final_texture(create_result_texture(device, resolution, format))
, m_temporary(create_result_texture(device, resolution, format))
, m_current_time(0.0f)
{
    if (parameter.name() != "bloom")
    {
        throw any_error("invalid parameter for bloom \"",
            string_wrapper(parameter.name()), "\"");
    }

    m_deviation = animatable_child<float>("deviation", parameter);
    m_multiplier = animatable_child<float>("multiplier", parameter);

    try
    {
        m_pixel_shader = compile_new_pixel_shader(device,
            pixel, "main_ps", "ps_3_0");

        m_vertex_shader = compile_new_vertex_shader(device,
            vertex, "main_vs", "vs_3_0");
    }
    catch(compilation_error& e)
    {
        e.attach_context(new exceptions::string_context("blur"));
        throw;
    }

    //m_blur.reset(new blur(device, resolution, D3DFMT_X8R8G8B8));

	//horizontal_strength_texture creation

    parameter_collection const& parameters = parameter.children();

	std::shared_ptr<parameter_node> horizontal_strength_texture_name_node
		= optional_child_node("horizontal_strength_texture", parameters);

	assert(horizontal_strength_texture_name_node);

	std::shared_ptr<animatable_value<std::string> const> horizontal_strength_texture_names
		= get_animatable<std::string>(*horizontal_strength_texture_name_node);

	std::shared_ptr<pt::math::step_curve_string_value_source const> horizontal_texture_name_step_curve
		= std::dynamic_pointer_cast<pt::math::step_curve_string_value_source const>(horizontal_strength_texture_names->source());

	d3d9::texture_2d_data_set horizontal_texture_2d_data_set = d3d9::build_texture_2d_data_set(
		*horizontal_texture_name_step_curve,
		device);

	// Create texture_2d_step_curve
	texture_2d_step_curve horizontal_texture_step_curve(horizontal_texture_2d_data_set);

	std::shared_ptr<texture_2d_source> horizontal_texture_source(new texture_2d_source(horizontal_texture_step_curve));

	m_horizontal_strength_texture.reset(
		new animatable_value<std::shared_ptr<texture_2d>>(
		horizontal_texture_source
		));

    // load texture filename curve, and create texture curve from that..

    std::shared_ptr<parameter_node> blur_strength_texture_name_node
        = optional_child_node("blur_strength_texture", parameters);

    assert(blur_strength_texture_name_node);

    std::shared_ptr<animatable_value<std::string> const> blur_strength_texture_names
        = get_animatable<std::string>(*blur_strength_texture_name_node);

    std::shared_ptr<pt::math::step_curve_string_value_source const> texture_name_step_curve
        = std::dynamic_pointer_cast<pt::math::step_curve_string_value_source const>(blur_strength_texture_names->source());

    d3d9::texture_2d_data_set texture_2d_data_set = d3d9::build_texture_2d_data_set(
        *texture_name_step_curve,
        device);

    // Create texture_2d_step_curve
    texture_2d_step_curve step_curve(texture_2d_data_set);

    std::shared_ptr<texture_2d_source> source(new texture_2d_source(step_curve));

    std::shared_ptr<animatable_value<std::shared_ptr<texture_2d>> const> blur_strength(
        new animatable_value<std::shared_ptr<texture_2d>>(
        source
        ));


    m_blur.reset(new blur(device, resolution, D3DFMT_A16B16G16R16F, blur_strength));
}

void bloom::update(float time)
{
    m_current_time = time;
    m_blur->update(time);
}

void bloom::draw(texture_2d const& source)
{
    graphics_device& device = source.mutable_device();
	
	std::shared_ptr<texture_2d> horizontal_strength_texture = m_horizontal_strength_texture->value_at(m_current_time);

    // Blur source texture.
    m_blur->draw(source);

    vector2i resolution = m_blur->resolution();

    std::vector<float> offsets_f;
    std::vector<vector4f> offsets;
    std::vector<vector4f> weights;

    m_pixel_shader->use();
    m_vertex_shader->use();

    set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    set_sampler_state(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	set_sampler_state(device, 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	set_sampler_state(device, 1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	set_sampler_state(device, 1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    //float multiplier = 3.0f;
    //float deviation = 2.0f;

    float multiplier = m_multiplier->value_at(m_current_time);
    float deviation = m_deviation->value_at(m_current_time);

    // horizontal
    {
        set_sample_offsets(resolution[0], offsets_f, weights, multiplier,
            deviation);
        offsets.resize(offsets_f.size());
        for (size_t i = 0; i < offsets_f.size(); ++i)
        {
            offsets[i] = vector2f(offsets_f[i], 0);
        }

        scoped_render_target_texture rt(m_temporary);
        set_texture(m_blur->result(), 0);
		set_texture(*horizontal_strength_texture, 1);

        constant const& c0 = m_pixel_shader->constants().by_name("offsets");
        constant const& c1 = m_pixel_shader->constants().by_name("weights");
        set_constant(c0, m_pixel_shader->constants(), offsets);
        set_constant(c1, m_pixel_shader->constants(), weights);

        fill_fullscreen_quad(device);
		set_texture(device, 1, com_ptr<IDirect3DTexture9>());
    }

    // vertical
    {
        set_sample_offsets(resolution[1], offsets_f, weights, multiplier,
            deviation);

        offsets.resize(offsets_f.size());
        for (size_t i = 0; i < offsets_f.size(); ++i)
        {
            offsets[i] = vector2f(0, offsets_f[i]);
        }

        scoped_render_target_texture rt(m_final_texture);
        set_texture(m_temporary, 0);

        constant const& c0 = m_pixel_shader->constants().by_name("offsets");
        constant const& c1 = m_pixel_shader->constants().by_name("weights");
        set_constant(c0, m_pixel_shader->constants(), offsets);
        set_constant(c1, m_pixel_shader->constants(), weights);

        fill_fullscreen_quad(device);
    }
}

texture_2d const& bloom::result() const
{
    return m_final_texture;
}

}}} // namespace pt::d3d9::rendering
