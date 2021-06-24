#ifndef PT_D3D9_RENDERING_SCALING_H
#define PT_D3D9_RENDERING_SCALING_H

#include "transformation.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class scaling final : public transformation
{
public:
    scaling(animatable_value<vector3f> const& scale);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private:
    animatable_value<vector3f> m_scale;
};

}}} // namespace pt::d3d9::rendering

#endif
