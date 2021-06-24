#ifndef PT_D3D9_RENDERING_RGB_CORRECTION_H
#define PT_D3D9_RENDERING_RGB_CORRECTION_H

#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/boost/noncopyable.h"
#include "pt/animatable_value.h"
#include "pt/boost/shared_ptr.h"

namespace pt {
    class parameter_node;
}

namespace pt { namespace d3d9 {
    class pixel_shader;
    class vertex_shader;
}}

namespace pt { namespace d3d9 { namespace rendering {

class rgb_correction : noncopyable
{
public:
    rgb_correction(graphics_device& device,
        shared_ptr<parameter_node const> parameter);

    void update(float time);
    void draw(texture_2d const& source);

private: // data members
    shared_ptr<pixel_shader> m_pixel_shader;
    shared_ptr<vertex_shader> m_vertex_shader;
    
    shared_ptr<d3d9::texture_2d> m_noise_texture;

    float m_current_time;
    shared_ptr<animatable_value<float> const> m_red_corr;
    shared_ptr<animatable_value<float> const> m_green_corr;
    shared_ptr<animatable_value<float> const> m_blue_corr;
};

}}} // namespace pt::d3d9::rendering

#endif
