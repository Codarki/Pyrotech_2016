#ifndef PT_D3D9_RENDERING_REFLECTION_H
#define PT_D3D9_RENDERING_REFLECTION_H

#include "transformation.h"
#include "pt/math/plane.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class reflection final : public transformation
{
public:
    reflection(std::shared_ptr<transformation> other_transformation,
        animatable_value<math::plane> reflection_plane_in_world);
        //std::shared_ptr<transformation> reflection_plane_item);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private:
    std::shared_ptr<transformation> m_other_transformation;
    //std::shared_ptr<transformation> m_reflection_plane_item;
    animatable_value<math::plane> m_reflection_plane_in_world;
};

}}} // namespace pt::d3d9::rendering

#endif
