#include "light_scattering.h"
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
#include "pt/file_input_stream.h"
#include "pt/data_stream_content.h"

namespace pt {
	namespace d3d9 {
		namespace rendering {

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

			light_scattering::light_scattering(graphics_device& device,
				parameter_node const& parameter, vector2i resolution,
				D3DFORMAT format)
				: m_final_texture(create_result_texture(device, resolution, format))
			{

				if (parameter.name() != "light_scattering")
				{
					throw any_error("invalid parameter for light_scattering \"",
						string_wrapper(parameter.name()), "\"");
				}
                m_enabled = animatable_child<float>("enabled", parameter);
                m_exposure = animatable_child<float>("exposure", parameter);
                m_decay = animatable_child<float>("decay", parameter);
                m_density = animatable_child<float>("density", parameter);
                m_weight = animatable_child<float>("weight", parameter);
                m_lightScrPosX = animatable_child<float>("lightScrPosX", parameter);
                m_lightScrPosY = animatable_child<float>("lightScrPosY", parameter);

				// Load HLSL
				std::string hlsl;
				{
					pt::path shader_path("data/shaders/hlsl/light_scattering.hlsl");
					file_input_stream file_stream(shader_path);
					data_stream_content data(file_stream);

					char const* ptr = reinterpret_cast<char const*>(data.data());
					std::string shader_file_text(ptr, data.size());
					assert(shader_file_text.c_str()[data.size()] == 0);

					hlsl += (pt::format("#line 1 \"%1%\"\n") % shader_path.string()).move_string();
					hlsl += shader_file_text;
				}


				try
				{
					m_pixel_shader = compile_new_pixel_shader(device,
						hlsl, "main_ps", "ps_3_0");

					m_vertex_shader = compile_new_vertex_shader(device,
						hlsl,//pass_through_vertex_shader_hlsl_with_texcoord(),
						"main_vs", "vs_3_0");
				}
				catch (compilation_error& e)
				{
					e.attach_context(new exceptions::string_context("light_scattering"));
					throw;
				}
			}

			void light_scattering::update(float time)
			{
				m_current_time = time;
			}

			void light_scattering::draw(texture_2d const& source)
			{
				if (static_cast<int>(m_enabled->value_at(m_current_time)) == 0) {
					m_isEnabled = static_cast<int>(m_enabled->value_at(m_current_time));
					return;
				}
				m_isEnabled = 1;
				//std::shared_ptr<d3d9::texture_2d> depth_text = d3d9::default_texture_2d_collection().find("DepthTexture");

				set_texture(source, 0);
				//set_texture(source2, 1);
				//set_texture(*depth_text, 2);


				m_pixel_shader->use();
				m_vertex_shader->use();

				graphics_device& device = source.mutable_device();

				set_sampler_state(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				set_sampler_state(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
				set_sampler_state(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
				/*set_sampler_state(device, 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				set_sampler_state(device, 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
				set_sampler_state(device, 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
				set_sampler_state(device, 2, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				set_sampler_state(device, 2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
				set_sampler_state(device, 2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
				*/
				if (m_pixel_shader->constants().count() > 0)
				{

					scoped_render_target_texture rt(m_final_texture);

					float exposure     = m_exposure->value_at(m_current_time);
					float decay        = m_decay->value_at(m_current_time);
					float density      = m_density->value_at(m_current_time);
					float weight       = m_weight->value_at(m_current_time);
					float lightScrPosX = m_lightScrPosX->value_at(m_current_time);
					float lightScrPosY = m_lightScrPosY->value_at(m_current_time);

					constant const& c1 = m_pixel_shader->constants().by_name("exposure");
					constant const& c2 = m_pixel_shader->constants().by_name("decay");
					constant const& c3 = m_pixel_shader->constants().by_name("density");
					constant const& c4 = m_pixel_shader->constants().by_name("weight");
					constant const& c5 = m_pixel_shader->constants().by_name("lightScrPosX");
					constant const& c6 = m_pixel_shader->constants().by_name("lightScrPosY");					

					set_constant(c1, m_pixel_shader->constants(), exposure);
					set_constant(c2, m_pixel_shader->constants(), decay);
					set_constant(c3, m_pixel_shader->constants(), density);
					set_constant(c4, m_pixel_shader->constants(), weight);
					set_constant(c5, m_pixel_shader->constants(), lightScrPosX);
					set_constant(c6, m_pixel_shader->constants(), lightScrPosY);

					fill_fullscreen_quad(device);
				}

				set_texture(device, 0, com_ptr<IDirect3DTexture9>());
				//set_texture(device, 1, com_ptr<IDirect3DTexture9>());
				//set_texture(device, 2, com_ptr<IDirect3DTexture9>());
			}

			texture_2d const& light_scattering::result() const
			{
				return m_final_texture;
			}

			int light_scattering::isEnabled() {
				return m_isEnabled;
			}

		}
	}
} // namespace pt::d3d9::rendering
