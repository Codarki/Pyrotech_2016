#ifndef PT_D3D9_RENDERING_BRIGHT_PASS_FILTER_H
#define PT_D3D9_RENDERING_BRIGHT_PASS_FILTER_H

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

class bright_pass_filter
{
public:
    bright_pass_filter(
        std::shared_ptr<pixel_shader>&& pixel_shader,
        std::shared_ptr<vertex_shader>&& vertex_shader,
        texture_2d&& texture,
        animatable_value<float>&& adapted_luminance,
        animatable_value<float>&& middle_gray,
        animatable_value<float>&& threshold,
        animatable_value<float>&& offset,
        animatable_value<float>&& white_point);


    bright_pass_filter(bright_pass_filter const&) = delete;
    bright_pass_filter const& operator=(bright_pass_filter const&) = delete;

    void update(float time);
    void draw(texture_2d const& source);

    texture_2d const& result() const;

private: // data members
	std::shared_ptr<pixel_shader> m_pixel_shader;
	std::shared_ptr<vertex_shader> m_vertex_shader;

    texture_2d m_texture;
    float m_current_time;

    animatable_value<float> const m_adapted_luminance;
    animatable_value<float> const m_middle_gray;
    animatable_value<float> const m_threshold;
    animatable_value<float> const m_offset;
    animatable_value<float> const m_white_point;
};

std::unique_ptr<bright_pass_filter> build_bright_pass_filter(graphics_device& device,
    parameter_node const& parameter, vector2i resolution,
    D3DFORMAT format);

}}} // namespace pt::d3d9::rendering

#endif
