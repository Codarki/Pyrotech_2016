#include "reflection_plane_in_world_source.h"
#include "transformation.h"
#include "pt/matrix_operations.h"
#include "pt/math/plane.h"
#include "pt/string_wrapper.h"

namespace pt { namespace d3d9 { namespace rendering {

reflection_plane_in_world_source::reflection_plane_in_world_source(
    std::shared_ptr<transformation> reflection_transformation)
: m_reflection_transformation(reflection_transformation)
{
}

bool reflection_plane_in_world_source::is_constant() const
{
    return m_reflection_transformation->is_constant();
}

math::plane reflection_plane_in_world_source::value_at(float time) const
{
    //time;
    //return math::make_plane(vector3f(0, 0, 1), vector3f(0, 1, 0));

    auto reflection_local_to_world = m_reflection_transformation->local_to_world_at(time);
    auto origo = pt::translation(reflection_local_to_world);
    auto normal = basis_axis_y(reflection_local_to_world);
    normal = normalize(normal);
    auto plane = math::make_plane(origo, normal);
    return plane;
}

string_wrapper reflection_plane_in_world_source::type_name() const
{
    return "reflection_plane_in_world_source";
}

//

plane_to_local_space_source::plane_to_local_space_source(
    animatable_value<math::plane> plane_in_world,
    std::shared_ptr<transformation> transformation)
: m_plane_in_world(plane_in_world)
, m_transformation(transformation)
{
}

bool plane_to_local_space_source::is_constant() const
{
    return m_plane_in_world.is_constant()
        && m_transformation->is_constant();
}

math::plane plane_to_local_space_source::value_at(float time) const
{
    //time;
    //return math::make_plane(vector3f(0, 0, 1), vector3f(0, 1, 0));

    auto plane = m_plane_in_world.value_at(time);
    auto local_to_world = m_transformation->local_to_world_at(time);
    auto world_to_local = invert(local_to_world);
    //auto world_to_local = m_transformation->local_to_world_at(time);
    auto plane_in_local = transform(plane, local_to_world);
    return plane_in_local;


    //return m_plane_in_world.value_at(time);

    //auto local_to_world = m_transformation->local_to_world_at(time);
    ////auto world_to_local = invert(local_to_world);
    //auto origo = pt::translation(local_to_world);
    //auto normal = -basis_axis_y(local_to_world);
    //normal = normalize(normal);
    //auto plane = math::make_plane(origo, normal);


    //auto plane = math::make_plane(vector3f(0, 0, 0), vector3f(0, -1, 0));
    //return plane;


    //auto local_to_world = m_transformation->local_to_world_at(time);
    //return transform(plane, invert(local_to_world));

    //reflection_plane_item_local_to_world = invert(reflection_plane_item_local_to_world);
    //auto origo = pt::translation(reflection_plane_item_local_to_world);
    //auto normal = basis_axis_y(reflection_plane_item_local_to_world);
    //normal = normalize(normal);
    //auto plane = math::make_plane(origo, normal);

}

string_wrapper plane_to_local_space_source::type_name() const
{
    return "plane_to_local_space_source";
}

}}}
