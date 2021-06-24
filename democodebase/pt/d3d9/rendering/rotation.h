#ifndef PT_D3D9_RENDERING_ROTATION_H
#define PT_D3D9_RENDERING_ROTATION_H

#include "transformation.h"
#include "pt/math/quaternion.h"
#include "pt/math/data_set.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class rotation final : public transformation
{
public:
    rotation();
    rotation(animatable_value<math::quaternionf> const& quaternion);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private:
    animatable_value<math::quaternionf> m_quaternion;
    mutable float m_time;
    mutable matrix4x4f m_local_to_parent;
};

//animatable_value<math::quaternionf> quaternion_value_from_axis_angles(
//    math::data_set<float> data[4]);

}}} // namespace pt::d3d9::rendering

#endif
