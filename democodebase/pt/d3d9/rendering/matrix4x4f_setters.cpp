#include "matrix4x4f_setters.h"
#include "projection_matrix.h"
#include "transformation.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/matrix_operations.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

// local_to_world_setter

local_to_world_setter::local_to_world_setter(
    transformation const& transformation)
: m_transformation(transformation)
{
}

void local_to_world_setter::update_target(float time) const
{
    matrix4x4f m = m_transformation.local_to_world_at(time);
    set_constant(constant(), constants(), m);
}

// world_to_local_setter

world_to_local_setter::world_to_local_setter(
    transformation const& transformation)
: m_transformation(transformation)
{
}

void world_to_local_setter::update_target(float time) const
{
    // TODO: cache invert in transformation.
    matrix4x4f m = invert(m_transformation.local_to_world_at(time));
    
   /*m = orthogonalize(m);
     vector3f x_axis = normalize(basis_axis_x(m));
    vector3f y_axis = normalize(basis_axis_y(m));
    vector3f z_axis = normalize(basis_axis_z(m));

    m.set_column(0, vector4f(x_axis, m.column(0)[3]));
    m.set_column(1, vector4f(y_axis, m.column(1)[3]));
    m.set_column(2, vector4f(z_axis, m.column(2)[3]));
    */
    set_constant(constant(), constants(), m);
}

// projection_setter

projection_setter::projection_setter(projection_matrix& projection)
: m_projection(projection)
{
}

void projection_setter::update_target(float time) const
{
    m_projection.update(time);
    set_constant(constant(), constants(), m_projection.get());
}

// world_to_view_clip_setter
world_to_view_clip_setter::world_to_view_clip_setter(
    transformation const& transformation,
    projection_matrix& projection)
: m_transformation(transformation)
, m_projection(projection)
{
}

void world_to_view_clip_setter::update_target(float time) const
{
    m_projection.update(time);

    // TODO thought the order was other way around..hmm
    matrix4x4f world_to_view_clip
        = invert(m_transformation.local_to_world_at(time))
        * m_projection.get();

    set_constant(constant(), constants(), world_to_view_clip);
}

// world_to_view_setter

world_to_view_setter::world_to_view_setter(
    transformation const& transformation)
: m_transformation(transformation)
{
}

void world_to_view_setter::update_target(float time) const
{
    matrix4x4f world_to_view
        = invert(m_transformation.local_to_world_at(time));

    set_constant(constant(), constants(), world_to_view);
}

}}} // namespace pt::d3d9::rendering
