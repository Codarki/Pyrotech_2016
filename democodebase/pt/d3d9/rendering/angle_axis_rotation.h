#ifndef PT_D3D9_RENDERING_ANGLE_AXIS_ROTATION_H
#define PT_D3D9_RENDERING_ANGLE_AXIS_ROTATION_H

#include "transformation.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class angle_axis_rotation final : public transformation
{
public:
    angle_axis_rotation(animatable_value<float> const& angle,
        animatable_value<vector3f> const& axis);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private: // data members
    animatable_value<float> m_angle;
    animatable_value<vector3f> m_axis;
};

}}} // namespace pt::d3d9::rendering

#endif
