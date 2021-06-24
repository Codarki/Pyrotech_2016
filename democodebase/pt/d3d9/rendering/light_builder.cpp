#include "light_builder.h"
#include "composite_variable_owner.h"
#include "light.h"
//#include "light_variables.h"
#include "material_cache.h"
#include "projection_matrix.h"
#include "transformer.h"
#include "view_variables.h"
#include "pt/d3d9/shaders/point_light_shader.h"
#include "pt/parameter_collection.h"
#include "pt/parameter_node_utility.h"
#include "pt/vector.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

// Free functions

std::unique_ptr<light> create_unique_light(
    transformation const& transformation,
    parameter_collection const& parameters,
    material_cache& material_cache,
    graphics_device& device)
{
    std::shared_ptr<parameter_node> const& node = *parameters.child("position");
    auto position_in_world = constant_value<vector3f>(*node);

    auto color = constant_value<vector3f>(**parameters.child("color"));
    auto range = constant_value<vector2f>(**parameters.child("range"))[1];

    auto is_spot_light = constant_value<bool>(**parameters.child("is_spot"));

    auto target_in_world = constant_value<vector3f>(**parameters.child("target"));
    auto hotspot = constant_value<float>(**parameters.child("hotspot"));
    auto falloff = constant_value<float>(**parameters.child("falloff"));

    float multiplier = constant_value<float>(**parameters.child("multiplier"));
    color *= multiplier;

    // TODO: What parameters?

    std::shared_ptr<animatable_value<float> const> animatable_falloff
        = get_animatable<float>(**(parameters.child("animatable_falloff")));
    std::shared_ptr<projection_matrix> projection(new projection_matrix(animatable_falloff));

    std::shared_ptr<light_properties> light_properties = material_cache.find_or_create_light(
        device, parameters);

    std::shared_ptr<rendering::view_from_light_variables> view_from_light_variables(new rendering::view_from_light_variables(transformation, *projection));
    //auto view_from_light_variables = std::make_shared<rendering::view_from_light_variables>(transformation, projection);

    // resolution will get overriden at scene_player.
    projection->set_resolution(vector2i(1024, 1024));

    std::shared_ptr<animatable_value<float> const> animatable_near_plane
        = get_animatable<float>(**(parameters.child("near_plane")));

    std::shared_ptr<animatable_value<float> const> animatable_far_plane
        = get_animatable<float>(**(parameters.child("far_plane")));

    float near_plane = animatable_near_plane->value_at(0);
    float far_plane = animatable_far_plane->value_at(0);

    projection->set_zplanes(near_plane, far_plane);

    auto is_casting_shadows = constant_value<bool>(**parameters.child("cast_shadows"));
    auto shadow_bias = constant_value<float>(**parameters.child("shadow_bias"));

    return std::make_unique<light>(
        light_properties,
        transformation,
        projection,
        view_from_light_variables,
        position_in_world,
        target_in_world,
        color,
        range,
        hotspot,
        falloff,
        is_spot_light,
        is_casting_shadows,
        shadow_bias);
}

}}} // namespace pt::d3d9::rendering
