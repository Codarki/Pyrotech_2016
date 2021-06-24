#ifndef PT_D3D9_RENDERING_BLOOM_H
#define PT_D3D9_RENDERING_BLOOM_H

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

class blur;

class bloom
{
public:
    bloom(graphics_device& device,
        parameter_node const& parameter, vector2i resolution,
        D3DFORMAT format);

    void update(float time);
    void draw(texture_2d const& source);

    texture_2d const& result() const;

private: // data members
    std::shared_ptr<pixel_shader> m_pixel_shader;
    std::shared_ptr<vertex_shader> m_vertex_shader;

    std::shared_ptr<blur> m_blur;

    float m_current_time;
    texture_2d m_final_texture;
    texture_2d m_temporary;

    std::shared_ptr<animatable_value<float> const> m_deviation;
    std::shared_ptr<animatable_value<float> const> m_multiplier;

	std::shared_ptr<animatable_value<std::shared_ptr<texture_2d>> const> m_horizontal_strength_texture;
};

}}} // namespace pt::d3d9::rendering

#endif
