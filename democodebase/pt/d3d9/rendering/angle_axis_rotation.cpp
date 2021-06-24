#include "angle_axis_rotation.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 { namespace rendering {

angle_axis_rotation::angle_axis_rotation(
    animatable_value<float> const& angle,
    animatable_value<vector3f> const& axis)
:   m_angle(angle)
,   m_axis(axis)
{
}

matrix4x4f angle_axis_rotation::get_local_to_parent_at(float time) const
{
    float angle = m_angle.value_at(time);
    vector3f axis = m_axis.value_at(time);

    return pt::rotation_matrix(axis, angle);
}

bool angle_axis_rotation::get_is_constant() const
{
    return m_angle.is_constant() && m_axis.is_constant();
}

}}} // namespace pt::d3d9::rendering
