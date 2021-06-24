#include "vector3f_setters.h"
#include "transformation.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/matrix_operations.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

// light_in_world_setter

light_in_world_setter::light_in_world_setter(
    transformation const& transformation)
: m_transformation(transformation)
{
}

void light_in_world_setter::update_target(float time) const
{
    vector3f t = translation(m_transformation.local_to_world_at(time));
    set_constant(constant(), constants(), t);
}

// light_dir_in_world_setter

light_dir_in_world_setter::light_dir_in_world_setter(
    transformation const& transformation)
: m_transformation(transformation)
{
}

void light_dir_in_world_setter::update_target(float time) const
{
    vector3f t = basis_axis_z(
        m_transformation.local_to_world_at(time));
    set_constant(constant(), constants(), t);
}

// view_in_world_setter

view_in_world_setter::view_in_world_setter(
    transformation const& transformation)
: m_transformation(transformation)
{
}

void view_in_world_setter::update_target(float time) const
{
    //vector3f t = translation(
    //    world_to_local_at(*m_transformer, time));
    vector3f t = translation(
        m_transformation.local_to_world_at(time));
    set_constant(constant(), constants(), t);
}

}}} // namespace pt::d3d9::rendering
