#ifndef PT_D3D9_RENDERING_DOF_H
#define PT_D3D9_RENDERING_DOF_H

#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/boost/noncopyable.h"
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

class dof : boost::noncopyable
{
public:
    dof(graphics_device& device,
        parameter_node const& parameter, vector2i resolution,
    D3DFORMAT format);

    void update(float time);
	void draw(texture_2d const& source, texture_2d const& source2);

	texture_2d const& result() const;
private: // data members
	texture_2d m_final_texture;
    std::shared_ptr<pixel_shader> m_pixel_shader;
    std::shared_ptr<vertex_shader> m_vertex_shader;

    float m_current_time;
    std::shared_ptr<animatable_value<float> const> m_focus;
    std::shared_ptr<animatable_value<float> const> m_range;
	std::shared_ptr<animatable_value<float> const> m_blur_rgb_dist;
	std::shared_ptr<animatable_value<float> const> m_gain;
	std::shared_ptr<animatable_value<float> const> m_threshold;
};

}}} // namespace pt::d3d9::rendering

#endif
