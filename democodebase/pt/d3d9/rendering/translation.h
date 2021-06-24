#ifndef PT_D3D9_RENDERING_TRANSLATION_H
#define PT_D3D9_RENDERING_TRANSLATION_H

#include "transformation.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class translation final : public transformation
{
public:
    translation(animatable_value<vector3f> value);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private:
    animatable_value<vector3f> m_translation;
};

}}} // namespace pt::d3d9::rendering

#endif
