#include "reflection.h"
#include "transformation.h"
#include "pt/matrix_operations.h"
#include "pt/math/plane.h"

namespace pt { namespace d3d9 { namespace rendering {

reflection::reflection(std::shared_ptr<transformation> other_transformation,
    animatable_value<math::plane> reflection_plane_in_world)
    //std::shared_ptr<transformation> reflection_plane_item)
: m_other_transformation(other_transformation)
//, m_reflection_plane_item(reflection_plane_item)
, m_reflection_plane_in_world(reflection_plane_in_world)
{
}

matrix4x4f reflection::get_local_to_parent_at(float time) const
{
    //auto reflection_plane_item_local_to_world = m_reflection_plane_item->local_to_world_at(time);
    //auto origo = pt::translation(reflection_plane_item_local_to_world);
    //auto normal = basis_axis_y(reflection_plane_item_local_to_world);
    //normal = normalize(normal);
    //auto plane = math::make_plane(origo, normal);

    auto plane = m_reflection_plane_in_world.value_at(time);

    matrix4x4f ref = reflect(plane);

    auto other_local_to_world = m_other_transformation->local_to_parent_at(time);

    return other_local_to_world * ref;
}

bool reflection::get_is_constant() const
{
    return m_other_transformation->is_constant()
        //&& m_reflection_plane_item->is_constant();
        && m_reflection_plane_in_world.is_constant();
}

}}} // namespace pt::d3d9::rendering
