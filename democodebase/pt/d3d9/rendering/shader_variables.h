#pragma once
#ifndef PT_D3D9_RENDERING_SHADER_VARIABLES_H
#define PT_D3D9_RENDERING_SHADER_VARIABLES_H

#include "simple_variable_owner.h"
#include "shader_parameters.h"
#include "pt/parameter_collection.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
class constant;
}}

namespace pt { namespace d3d9 { namespace rendering {

class shader_parameters;

class shader_variables : public simple_variable_owner
{
public:
    shader_variables(shader_parameters const& parameters);
    ~shader_variables();

    parameter_collection const& parameters() const;

protected: // interface required by simple_variable_owner
    std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const;
    std::unique_ptr<variable_setter> on_create_setter(
        constant_buffer const& constants, size_t index) const;

private: // helper functions
    std::unique_ptr<variable_setter> create_object(
        constant_buffer const& constants, constant const& c) const;
    std::unique_ptr<constant_setter> create_scalar(
        constant const& constant) const;
    std::unique_ptr<constant_setter> create_vector(
        constant const& constant) const;

private: // data members
    shader_parameters m_parameters;
};

}}} // namespace pt::d3d9::rendering

#endif
