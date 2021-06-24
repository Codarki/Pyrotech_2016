#pragma once
#ifndef PT_D3D9_RENDERING_VIEW_VARIABLES_H
#define PT_D3D9_RENDERING_VIEW_VARIABLES_H

#include "simple_variable_owner.h"
#include "matrix4x4f_setters.h"
#include "vector3f_setters.h"
#include "time_setter.h"
#include "animatable_setter.h"
#include "pt/d3d9/constant.h"
#include "pt/math/plane.h"

namespace pt { namespace d3d9 { namespace rendering {

class view_variables : public simple_variable_owner
{
public:
    view_variables(
        transformation const& transformation,
        projection_matrix& projection,
        std::vector<animatable_value<math::plane>> const& clipping_planes)
    : m_transformation(transformation)
    , m_projection(projection)
    , m_clipping_planes(clipping_planes)
    {
    }

protected: // interface required by simple_variable_owner
    std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const
    {
        if (constant.name() == "projection")
        {
            return std::make_unique<projection_setter>(m_projection);
        }
        else if (constant.name() == "time")
        {
            return std::make_unique<time_setter>();
        }
        else if (constant.name() == "world_to_view_clip")
        {
            return std::make_unique<world_to_view_clip_setter>(m_transformation, m_projection);
        }
        else if (constant.name() == "world_to_view")
        {
            return std::make_unique<world_to_view_setter>(m_transformation);
        }
        else if (constant.name() == "view_in_world")
        {
            return std::make_unique<view_in_world_setter>(m_transformation);
        }
        else if (constant.name() == "view_to_world")
        {
            return std::make_unique<local_to_world_setter>(m_transformation);
        }
        else if (constant.name() == "clip_planes_in_world")
        {
            // TODO: Animatable clipping planes
            std::vector<pt::vector4f> values;
            for (auto const& x : m_clipping_planes)
            {
                auto plane = x.value_at(0);
                values.push_back(plane.as_vector());
            }
            // Add clipping planes at infinity.
            for (size_t i = m_clipping_planes.size(); i < 4; ++i)
            {
                auto plane = math::make_plane(vector3f(0, -1000000.0f, 0), vector3f(0, 1, 0));
                values.push_back(plane.as_vector());
            }
            return std::make_unique<basic_array_setter<vector4f>>(values);
        }
        //else if (constant.name() == "world_to_view")
        //{
        //    return std::make_unique<world_to_local_setter>(m_transformation != nullptr ? m_transformation : m_transformer->shared_transformation());
        //}

        return nullptr;
    }
private: // data members
    transformation const& m_transformation;
    projection_matrix& m_projection;
    std::vector<animatable_value<math::plane>> m_clipping_planes;
};

/// frustum light projection for HLSL shader constant.
class view_from_light_variables : public simple_variable_owner
{
public:
    view_from_light_variables(
        transformation const& transformation,
        projection_matrix& projection)
        : m_transformation(transformation)
        , m_projection(projection)
    {
    }

protected: // interface required by simple_variable_owner
    std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const
    {
        if (constant.name() == "light_projection")
        {
            return std::make_unique<projection_setter>(m_projection);
        }
        else if (constant.name() == "world_to_light_clip")
        {
            return std::make_unique<world_to_view_clip_setter>(m_transformation, m_projection);
        }

        return nullptr;
    }
private: // data members
    transformation const& m_transformation;
    projection_matrix& m_projection;
};

}}} // namespace pt::d3d9::rendering

#endif
