#ifndef CONTENT_SEQUENCES_COLOR_FADE_H
#define CONTENT_SEQUENCES_COLOR_FADE_H

#include "simple_composite_sequence.h"
//#include "../spline_parameter.h"
#include "pt/d3d9/device_child.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/vector.h"
#include "pt/animatable_value.h"

namespace pt {
    class parameter_collection;
}

namespace pt { namespace d3d9 {
    class pixel_shader;
    class vertex_shader;
}}

namespace content { namespace sequences {

class color_fade : public simple_composite_sequence
{
public: // type definitions
    enum fade_type
    {
        cos_180_curve,
        pow_4_curve,
        inv_pow_4_curve,
        linear,
    };

public:
    color_fade(d3d9::graphics_device& device, config_node const& config,
        parameter_collection const& parameters);

private: // simple_composite_sequence virtual function overloads
	std::shared_ptr<d3d9::texture_2d> on_draw(rendertarget& target,
        float time, std::shared_ptr<d3d9::texture_2d> child_result);

public: // interface required by composite_sequence
    void parse(config_node const& config,
        parameter_collection const& parameters);

private: // data members
    float m_time;
    float m_fade_in_duration;
    float m_fade_out_duration;
    //float m_duration;
    float m_max_alpha;
    //spline_parameter<float> m_duration;
    fade_type m_type;

    std::shared_ptr<animatable_value<vector3f> const > m_color;
    std::shared_ptr<animatable_value<float> const> m_duration;

    std::shared_ptr<d3d9::pixel_shader> m_pixel_shader;
    std::shared_ptr<d3d9::vertex_shader> m_vertex_shader;

    d3d9::graphics_device* m_device;
};

}} // namespace content::sequences

#endif
