#pragma once
#ifndef PT_D3D9_RENDERING_SOURCE_LIGHT_MATERIAL_H
#define PT_D3D9_RENDERING_SOURCE_LIGHT_MATERIAL_H

#include "pt/d3d9/rendering/light_shader_parameters.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

class light_material
{
public:
    light_material(std::string const& name,
        light_shader_parameters const& shader_parameters);

    std::string const& name() const;
    light_shader_parameters const& shader_parameters() const;

private: // data members
    std::string m_name;
    light_shader_parameters m_shader_parameters;
};

}}}} // namespace pt::d3d9::rendering::source

#endif
