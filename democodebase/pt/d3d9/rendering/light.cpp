#include "light.h"
#include "light_properties.h"
#include "light_variables.h"
#include "composite_variable_owner.h"
#include "transformer.h"
#include "view_variables.h"
#include "pt/d3d9/shaders/point_light_shader.h"
#include "pt/format.h"
#include "pt/log.h"
#include "pt/parameter_collection.h"
#include "pt/parameter_node_utility.h"
#include "pt/matrix.h"
#include "pt/string_wrapper.h"
#include "pt/vector_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

light::light(
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
    float shadow_bias)
: m_light_properties(light_properties)
, m_projection_matrix(projection_matrix)
, m_view_from_light_variables(view_from_light_variables)
, m_variables(new composite_variable_owner)
, m_position_in_world(position_in_world)
, m_target_in_world(target_in_world)
, m_color(color)
, m_range(range)
, m_hotspot(hotspot)
, m_falloff(falloff)
, m_is_spot_light(is_spot_light)
, m_is_shadow_mapped(is_shadow_mapped)
, m_shadow_bias(shadow_bias)
{
    log(pt::format("Constructing light: color: %1%, range %2%, hotspot %3%, falloff %4%, is spot '%5%', is shadow mapped '%6%' ")
        % to_string(m_color) % m_range % m_hotspot % m_falloff % m_is_spot_light % m_is_shadow_mapped);

    // TODO: Should be composition of light_variables and light_properties variables.
    m_variables->push_back(std::make_shared<light_variables>(
        transformation, m_color, m_range, m_hotspot, m_falloff, m_is_spot_light, m_shadow_bias));

    m_variables->push_back(m_view_from_light_variables);
}

variable_owner& light::variables()
{
    return *m_variables;
}

variable_owner const& light::variables() const
{
    return *m_variables;
}

box3f light::bounding_box() const
{
    vector3f max_range(m_range, m_range, m_range);
    return box_from_min_max(-max_range, max_range);
}

rendering::light_properties const& light::light_properties() const
{
    return *m_light_properties;
}

rendering::light_properties& light::light_properties()
{
    return *m_light_properties;
}

std::shared_ptr<projection_matrix> light::projection() const
{
    return m_projection_matrix;
}

std::shared_ptr<shaders::light_shader const> light::shader() const
{
    return m_light_properties->shader();
}

bool light::is_spot_light() const
{
    // TODO: remove this method.
    return m_is_spot_light;
}

bool light::is_shadow_mapped() const
{
    return m_is_shadow_mapped;
}

}}} // namespace pt::d3d9::rendering

