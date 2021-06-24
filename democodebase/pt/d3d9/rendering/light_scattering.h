#ifndef PT_D3D9_RENDERING_LIGHT_SCATTERING_H
#define PT_D3D9_RENDERING_LIGHT_SCATTERING_H

#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/boost/noncopyable.h"
#include "pt/animatable_value.h"
#include "pt/std/memory.h"

namespace pt {
	class parameter_node;
}

namespace pt {
	namespace d3d9 {
		class pixel_shader;
		class vertex_shader;
	}
}

namespace pt {
	namespace d3d9 {
		namespace rendering {

			class light_scattering : boost::noncopyable
			{
			public:
				light_scattering(graphics_device& device,
					parameter_node const& parameter, vector2i resolution,
					D3DFORMAT format);

				void update(float time);
				void draw(texture_2d const& source);

				texture_2d const& result() const;

				int isEnabled();
			private: // data members
				texture_2d m_final_texture;
				int m_isEnabled;

				std::shared_ptr<pixel_shader> m_pixel_shader;
				std::shared_ptr<vertex_shader> m_vertex_shader;

				float m_current_time;
				std::shared_ptr<animatable_value<float> const> m_enabled;

				std::shared_ptr<animatable_value<float> const> m_exposure;
				std::shared_ptr<animatable_value<float> const> m_decay;
				std::shared_ptr<animatable_value<float> const> m_density;
				std::shared_ptr<animatable_value<float> const> m_weight;
				std::shared_ptr<animatable_value<float> const> m_lightScrPosX;
				std::shared_ptr<animatable_value<float> const> m_lightScrPosY;
			};

		}
	}
} // namespace pt::d3d9::rendering

#endif
