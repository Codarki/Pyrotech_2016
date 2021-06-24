#pragma once
#ifndef PT_D3D9_RENDERING_MATRIX4X4F_SETTERS_H
#define PT_D3D9_RENDERING_MATRIX4X4F_SETTERS_H

#include "constant_setter.h"

namespace pt { namespace d3d9 { namespace rendering {

class projection_matrix;
class transformation;

/// TODO Implement local-to-parent here when needed.

/// Sets local-to-world shader constant.
class local_to_world_setter final : public constant_setter
{
public:
    local_to_world_setter(transformation const& transformation);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    transformation const& m_transformation;
};

/// Sets world-to-local shader constant.
class world_to_local_setter final : public constant_setter
{
public:
    world_to_local_setter(transformation const& transformation);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    transformation const& m_transformation;
};

/// Sets view projection matrix.
class projection_setter final : public constant_setter
{
public:
    projection_setter(projection_matrix& projection);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    projection_matrix& m_projection;
};

/// Sets world-to-view-clip transformation matrix.
class world_to_view_clip_setter final : public constant_setter
{
public:
    world_to_view_clip_setter(transformation const& transformation,
        projection_matrix& projection);

public: // interface required by variable_setter
    void update_target(float time) const final override;

private: // data members
    transformation const& m_transformation;
    projection_matrix& m_projection;
};

/// Sets world-to-view transformation matrix.
class world_to_view_setter final : public constant_setter
{
public:
    world_to_view_setter(transformation const& transformation);

public: // interface required by variable_setter
    void update_target(float time) const;

private: // data members
    transformation const& m_transformation;
};

}}} // namespace pt::d3d9::rendering

#endif
