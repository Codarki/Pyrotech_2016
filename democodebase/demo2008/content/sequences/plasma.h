#ifndef CONTENT_SEQUENCES_PLASMA_H
#define CONTENT_SEQUENCES_PLASMA_H

#include "simple_composite_sequence.h"
#include "pt/d3d9/shaders/point_light_shader.h"
#include "pt/parameter_collection_fwd.h"
#include "pt/d3d9/device_child.h"

namespace pt { namespace d3d9 {
    class texture_2d_value;
    class constant_texture_2d_source;
}}

namespace pt { namespace d3d9 { namespace rendering {
    class material_cache;
    class surface;
    class shading_program;
}}}

namespace content { namespace sequences {

using namespace pt;

namespace detail {
    class time_variable;
}

class plasma : public simple_composite_sequence
{
public:
    plasma(d3d9::graphics_device& device, config_node const& config,
        d3d9::rendering::material_cache& materials,
        pt::parameter_collection const& parameters);

private: // simple_composite_sequence virtual function overloads
	std::shared_ptr<d3d9::texture_2d> on_draw(rendertarget& target,
        float time, std::shared_ptr<d3d9::texture_2d> child_result);

private: // data members
    d3d9::graphics_device* m_device;
    d3d9::shaders::dummy_light_shader m_dummy_light_shader;
    std::shared_ptr<d3d9::rendering::shading_program> m_program;
    std::shared_ptr<d3d9::rendering::surface> m_surface;
    std::shared_ptr<detail::time_variable> m_time;

    std::shared_ptr<d3d9::constant_texture_2d_source> m_child_texture_source;
    std::shared_ptr<d3d9::texture_2d_value> m_child_texture;
};

}} // namespace content::sequences

#endif
