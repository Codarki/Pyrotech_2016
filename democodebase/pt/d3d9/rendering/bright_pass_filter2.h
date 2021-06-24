#ifndef PT_D3D9_RENDERING_BRIGHT_PASS_FILTER2_H
#define PT_D3D9_RENDERING_BRIGHT_PASS_FILTER2_H

#include "pt/boost/noncopyable.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/animatable_value.h"
#include "pt/std/memory.h"

namespace pt {
    class parameter_node;
}

namespace pt { namespace d3d9 {
    class pixel_shader;
    class vertex_shader;
}}

namespace pt { namespace d3d9 { namespace rendering {

class bright_pass_filter2 : boost::noncopyable
{
public:
    bright_pass_filter2(graphics_device& device,
        parameter_node const& parameter, vector2i resolution,
        D3DFORMAT format);

    void update(float time);
    void draw(texture_2d const& source);

    texture_2d const& result() const;

private: // data members
	std::shared_ptr<pixel_shader> m_pixel_shader;
	std::shared_ptr<vertex_shader> m_vertex_shader;

    texture_2d m_texture;
    float m_current_time;

	std::shared_ptr<animatable_value<float> const> m_adapted_luminance;
	std::shared_ptr<animatable_value<float> const> m_middle_gray;
	std::shared_ptr<animatable_value<float> const> m_threshold;
	std::shared_ptr<animatable_value<float> const> m_offset;
};

}}} // namespace pt::d3d9::rendering

#endif
