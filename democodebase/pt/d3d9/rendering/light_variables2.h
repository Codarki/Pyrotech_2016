#pragma once
#ifndef PT_D3D9_RENDERING_LIGHT_VARIABLES2_H
#define PT_D3D9_RENDERING_LIGHT_VARIABLES2_H

#include "variable_owner.h"
#include "shader_variables.h"
#include "light_shader_parameters.h"

namespace pt { namespace d3d9 {
    class constant;
}}

namespace pt { namespace d3d9 { namespace rendering {

class light_variables2 : public variable_owner
{
public:
    light_variables2(light_shader_parameters const& parameters);
    ~light_variables2();

    std::shared_ptr<shaders::light_shader const> shader() const;

    parameter_collection const& parameters() const;

public: // interface required by variable_owner
    bool try_create_constants(constant_buffer const& constants);
    void update_target(float time, constant_buffer const& buffer) const;

private: // data members
    std::shared_ptr<shaders::light_shader const> m_shader;
    shader_variables m_shader_variables;
};

}}} // namespace pt::d3d9::rendering

#endif
