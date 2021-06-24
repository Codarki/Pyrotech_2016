#pragma once
#ifndef PT_D3D9_RENDERING_SOURCE_MATERIAL_H
#define PT_D3D9_RENDERING_SOURCE_MATERIAL_H

#include "pt/std/vector.h"
#include "pt/d3d9/rendering/surface_shader_parameters.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

class material_shader
{
public:
    material_shader(
        surface_shader_parameters const& shader_parameters,
        surface_shader_parameters const& pre_shader_parameters);

    surface_shader_parameters const& shader_parameters() const;
    surface_shader_parameters const& pre_shader_parameters() const;

private: // data members
    surface_shader_parameters m_shader_parameters;
    surface_shader_parameters m_pre_shader_parameters;
};

class material
{
public:
    material(std::string const& name,
        std::vector<material_shader> shaders);

    std::string const& name() const;
    std::vector<material_shader> const& shaders() const;

private: // data members
    std::string m_name;
    std::vector<material_shader> m_shaders;
};

}}}} // namespace pt::d3d9::rendering::source

#endif
