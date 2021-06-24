#pragma once
#ifndef PT_D3D9_RENDERING_SHADER_PARAMETERS_H
#define PT_D3D9_RENDERING_SHADER_PARAMETERS_H

#include "pt/parameter_collection.h"
#include "pt/config_node.h"

namespace pt { namespace d3d9 { namespace rendering {

class shader_parameters
{
public:
    shader_parameters(parameter_collection const& parameters,
        config_node const& sampler_states);

    parameter_collection const& parameters() const;
    config_node const& sampler_states() const;

private: // data members
    parameter_collection m_parameters;
    config_node m_sampler_states;
};

}}} // namespace pt::d3d9::rendering

#endif
