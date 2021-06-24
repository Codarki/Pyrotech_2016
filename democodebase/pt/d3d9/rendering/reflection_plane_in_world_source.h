#pragma once
#ifndef PT_D3D9_RENDERING_REFLECTION_PLANE_IN_WORLD_SOURCE__H
#define PT_D3D9_RENDERING_REFLECTION_PLANE_IN_WORLD_SOURCE__H

#include "pt/math/plane.h"
#include "pt/animatable_value.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class transformation;

class reflection_plane_in_world_source : public animatable_value_source<math::plane>
{
public:
    reflection_plane_in_world_source(
        std::shared_ptr<transformation> reflection_transformation);

    bool is_constant() const final override;
    math::plane value_at(float time) const final override;
    string_wrapper type_name() const final override;

private:
    std::shared_ptr<transformation> m_reflection_transformation;
};

class plane_to_local_space_source : public animatable_value_source<math::plane>
{
public:
    plane_to_local_space_source(
        animatable_value<math::plane> plane_in_world,
        std::shared_ptr<transformation> transformation);

    bool is_constant() const final override;
    math::plane value_at(float time) const final override;
    string_wrapper type_name() const final override;

private:
    animatable_value<math::plane> m_plane_in_world;
    std::shared_ptr<transformation> m_transformation;
};

}}}

#endif
