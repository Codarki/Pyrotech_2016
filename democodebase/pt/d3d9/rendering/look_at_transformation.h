#ifndef PT_D3D9_RENDERING_LOOK_AT_TRANSFORMATION_H
#define PT_D3D9_RENDERING_LOOK_AT_TRANSFORMATION_H

#include "transformation.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class look_at_transformation final : public transformation
{
public:
    look_at_transformation(
        std::shared_ptr<transformation> position,
        std::shared_ptr<transformation> target);
    look_at_transformation(
        std::shared_ptr<transformation> position,
        std::shared_ptr<transformation> target,
        std::shared_ptr<animatable_value<float> > roll);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private: // helper functions
    matrix4x4f parent_to_local_at(float time) const;

private:
    std::shared_ptr<transformation> m_position;
    std::shared_ptr<transformation> m_target;
    std::shared_ptr<animatable_value<float> > m_roll;
    mutable float m_current_time;
    mutable matrix4x4f m_local_to_parent;
    mutable matrix4x4f m_parent_to_local;
};

}}} // namespace pt::d3d9::rendering

#endif
