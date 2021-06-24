#pragma once
#ifndef PT_D3D9_RENDERING_VECTOR3F_SETTERS_H
#define PT_D3D9_RENDERING_VECTOR3F_SETTERS_H

#include "constant_setter.h"

namespace pt { namespace d3d9 { namespace rendering {

    class transformation;

/// Sets light position vector in world space.
class light_in_world_setter final : public constant_setter
{
public:
    light_in_world_setter(transformation const& transformation);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    transformation const& m_transformation;
};

/// Sets light direction vector in world space.
class light_dir_in_world_setter final : public constant_setter
{
public:
    light_dir_in_world_setter(transformation const& transformation);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    transformation const& m_transformation;
};

/// Sets view position vector in world.
class view_in_world_setter final : public constant_setter
{
public:
    view_in_world_setter(transformation const& transformation);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    transformation const& m_transformation;
};

}}} // namespace pt::d3d9::rendering

#endif
