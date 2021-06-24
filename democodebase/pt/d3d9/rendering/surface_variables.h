#pragma once
#ifndef PT_D3D9_RENDERING_SURFACE_VARIABLES_H
#define PT_D3D9_RENDERING_SURFACE_VARIABLES_H

#include "variable_owner.h"
#include "shader_variables.h"
#include "surface_shader_parameters.h"

namespace pt { namespace d3d9 {
class constant;
}}

namespace pt { namespace d3d9 { namespace rendering {

class surface_variables : public variable_owner
{
public:
    surface_variables(surface_shader_parameters const& parameters);
    ~surface_variables();

	std::shared_ptr<shaders::surface_shader const> shader() const;

    parameter_collection const& parameters() const;

public: // interface required by variable_owner
    bool try_create_constants(constant_buffer const& constants);
    void update_target(float time, constant_buffer const& buffer) const;

private: // data members
    std::shared_ptr<shaders::surface_shader const> m_shader;
    shader_variables m_shader_variables;
};

}}} // namespace pt::d3d9::rendering

#endif
