#ifndef PT_D3D9_RENDERING_NOISE_H
#define PT_D3D9_RENDERING_NOISE_H

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

class noise
{
public:
    noise(graphics_device& device, parameter_node const& parameter);

    noise(noise const&) = delete;
    noise const& operator=(noise const&) = delete;

    void update(float time);
    void draw(texture_2d const& source);

private: // data members
    std::shared_ptr<pixel_shader> m_pixel_shader;
    std::shared_ptr<vertex_shader> m_vertex_shader;
    
    std::shared_ptr<d3d9::texture_2d> m_noise_texture;

    float m_current_time;
    std::shared_ptr<animatable_value<float> const> m_noise_strength;
    std::shared_ptr<animatable_value<float> const> m_noise_speed;
    std::shared_ptr<animatable_value<float> const> m_noise_frequency;
};

}}} // namespace pt::d3d9::rendering

#endif
