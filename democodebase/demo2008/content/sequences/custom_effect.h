#ifndef CONTENT_SEQUENCES_CUSTOM_EFFECT_H
#define CONTENT_SEQUENCES_CUSTOM_EFFECT_H

#include "simple_composite_sequence.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/vector.h"

namespace pt {
    template<typename T>
    class animatable_value;
    class parameter_collection;
}

namespace pt { namespace d3d9 {
    class pixel_shader;
    class vertex_shader;
}}

namespace content { namespace sequences {

class custom_effect : public simple_composite_sequence
{
public:
    custom_effect(d3d9::graphics_device& device, config_node const& config,
        parameter_collection const& parameters);
    custom_effect(d3d9::graphics_device& device,
        std::shared_ptr<composite_sequence> input, config_node const& config,
        parameter_collection const& parameters);

private: // simple_composite_sequence virtual function overloads
    std::shared_ptr<d3d9::texture_2d> on_draw(rendertarget& target,
        float time, std::shared_ptr<d3d9::texture_2d> child_result);

private: // helper functions
    void parse(config_node const& config,
        parameter_collection const& parameters);
    void init(d3d9::graphics_device& device);

private: // data members
    std::string m_name;
    float m_speed_rot;
    float m_speed_zoom;
    std::shared_ptr<animatable_value<float> const > m_alpha;
    std::shared_ptr<animatable_value<float> const > m_intensity;
    std::string m_input_name;

    std::shared_ptr<animatable_value<vector2f> const > m_center;
    std::shared_ptr<animatable_value<vector2f> const > m_size;

    std::shared_ptr<d3d9::pixel_shader> m_pixel_shader;
    std::shared_ptr<d3d9::vertex_shader> m_vertex_shader;
};

}} // namespace content::sequences

#endif

