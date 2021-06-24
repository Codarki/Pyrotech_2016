#ifndef PT_D3D9_RENDERING_SHADER_PARAMETER_BUILDER_H
#define PT_D3D9_RENDERING_SHADER_PARAMETER_BUILDER_H

#include "surface_shader_parameters.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/config_node.h"

namespace pt {
    class animatable_value_base;
}

namespace pt { namespace d3d9 { namespace rendering { namespace source {
    class shader_description;
}}}}

namespace pt { namespace d3d9 { namespace rendering {

class shader_parameter_builder
{
public:
    shader_parameter_builder(
        config_node shader_parameters,
        config_node sampler_states,
        parameter_collection const& shared_parameters,
        graphics_device& device);

    shader_parameters build(
        source::shader_description const& shader_description,
        parameter_collection const& parameter_overrides) const;

private: // helper functions
    std::shared_ptr<parameter_node> build_parameter(
        config_node const& shader_parameter,
        source::shader_description const& shader_description,
        parameter_collection const& parameter_overrides) const;

    std::shared_ptr<animatable_value_base> value_for(
        config_node const& material_parameter,
        config_node const& shader_parameter_config) const;
    std::shared_ptr<animatable_value_base> default_value_for(
        config_node const& shader_parameter) const;

private: // data members
    config_node m_shader_parameters;
    config_node m_sampler_states;
    parameter_collection m_shared_parameters;
    graphics_device* m_device;
};

}}} // namespace pt::d3d9::rendering

#endif
