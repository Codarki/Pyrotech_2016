#ifndef PT_D3D9_RENDERING_SRT_TRANSFORMATION_H
#define PT_D3D9_RENDERING_SRT_TRANSFORMATION_H

#include "transformation.h"
#include "rotation.h"
#include "scaling.h"
#include "translation.h"

namespace pt { namespace d3d9 { namespace rendering {

/// Scale-Rotation-Translation transformation.
class srt_transformation final : public transformation
{
public:
    srt_transformation(scaling scale, rotation rotation,
        translation translation);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private:
    matrix4x4f impl_local_to_parent_at(float time) const;
    bool impl_is_constant() const;

private:
    scaling m_scale;
    rotation m_rotation;
    translation m_translation;

    mutable float m_time;
    mutable matrix4x4f m_local_to_parent;
};

/// Scale-Rotation-Translation transformation.
class pivoted_srt_transformation final : public transformation
{
public:
    pivoted_srt_transformation(scaling scale, rotation rotation,
        translation translation, vector3f pivot);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private:
    scaling m_scale;
    rotation m_rotation;
    translation m_translation;

    vector3f m_pivot;
    matrix4x4f m_pivot_translation;
};

}}} // namespace pt::d3d9::rendering

#endif
