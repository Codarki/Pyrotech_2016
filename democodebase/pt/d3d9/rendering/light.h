#ifndef PT_D3D9_RENDERING_LIGHT_H
#define PT_D3D9_RENDERING_LIGHT_H

#include "item.h"
#include "pt/box.h"
#include "pt/vector.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {
    class light_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering {

class composite_variable_owner;
class light_properties;
class projection_matrix;
class transformation;
class variable_owner;
class view_from_light_variables;

class light
{
public:
    light(
        std::shared_ptr<rendering::light_properties> light_properties,
        transformation const& transformation,
        std::shared_ptr<projection_matrix> projection_matrix,
        std::shared_ptr<rendering::view_from_light_variables> view_from_light_variables,
        vector3f position_in_world,
        vector3f target_in_world,
        vector3f color,
        float range,
        float hotspot,
        float falloff,
        bool is_spot_light,
        bool is_shadow_mapped,
        float shadow_bias);

public:
    variable_owner& variables();
    variable_owner const& variables() const;
    box3f bounding_box() const;

    rendering::light_properties const& light_properties() const;
    rendering::light_properties& light_properties();
    std::shared_ptr<projection_matrix> projection() const;
    std::shared_ptr<shaders::light_shader const> shader() const;

    bool is_spot_light() const;
    bool is_shadow_mapped() const;

protected: // data members
	std::shared_ptr<composite_variable_owner> m_variables;

private: // data members
    std::shared_ptr<rendering::light_properties> m_light_properties;
    std::shared_ptr<projection_matrix> m_projection_matrix;
    std::shared_ptr<rendering::view_from_light_variables> m_view_from_light_variables;
    vector3f m_position_in_world;
	vector3f m_target_in_world;
    vector3f m_color;
    float m_range;
	float m_hotspot;
	float m_falloff;
    bool m_is_spot_light;
    bool m_is_shadow_mapped;
    float m_shadow_bias;
};

}}} // namespace pt::d3d9::rendering

#endif