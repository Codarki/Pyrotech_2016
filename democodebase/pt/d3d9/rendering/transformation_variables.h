#pragma once
#ifndef PT_D3D9_RENDERING_TRANSFORMATION_VARIABLES_H
#define PT_D3D9_RENDERING_TRANSFORMATION_VARIABLES_H

#include "simple_variable_owner.h"
#include "matrix4x4f_setters.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

class transformation;

/// Handles creation of transformation variable setters for shaders.
class transformation_variables : public simple_variable_owner
{
public:
    transformation_variables(transformation const& transformation)
    : m_transformation(transformation)
    , m_local_to_world_name("object_to_world")
    , m_world_to_local_name("world_to_object")
    {
    }

    /// Name of the matrix in hlsl.
    void set_local_to_world_name(std::string const& name)
    {
        m_local_to_world_name = name;
    }
    void set_world_to_local_name(std::string const& name)
    {
        m_world_to_local_name = name;
    }

protected: // interface required by simple_variable_owner
    std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const
    {
        if (constant.name() == m_local_to_world_name)
        {
            return std::make_unique<local_to_world_setter>(m_transformation);
        }
        if (constant.name() == m_world_to_local_name)
        {
            return std::make_unique<world_to_local_setter>(m_transformation);
        }
        return nullptr;
    }

private: // data members
    std::string m_local_to_world_name;
    std::string m_world_to_local_name;
    transformation const& m_transformation;
};

}}} // namespace pt::d3d9::rendering

#endif
