#ifndef PT_D3D9_RENDERING_SOURCE_MATERIAL_BUILDER_H
#define PT_D3D9_RENDERING_SOURCE_MATERIAL_BUILDER_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"

namespace pt {
    class animatable_value_base;
    class parameter_collection;
}

namespace pt { namespace d3d9 { namespace shaders {
    class light_shader;
    class light_shader_collection;
    class surface_shader;
    class surface_shader_collection;
}
}
}

namespace pt { namespace d3d9 { namespace rendering { namespace source {

class light_material;
class material;
class material_description;

class material_builder
{
public:
    material_builder();
    material_builder(
        std::unique_ptr<material_description> description,
        std::vector<std::shared_ptr<shaders::surface_shader>> shaders,
        std::vector<std::shared_ptr<shaders::surface_shader>> pre_shaders);
    ~material_builder();

    std::string const& name() const;
    std::string const& p3d_material_name() const;

    void build(parameter_collection const& parameter_overrides,
        parameter_collection const& shared_parameters,
        graphics_device& device);

	std::shared_ptr<material> built_material();

private: // data member
    std::shared_ptr<material_description> m_description;
    std::vector<std::shared_ptr<shaders::surface_shader>> m_shaders;
    std::vector<std::shared_ptr<shaders::surface_shader>> m_pre_shaders;
    std::shared_ptr<material> m_material;
};

class light_material_builder
{
public:
    light_material_builder();
    light_material_builder(
        std::unique_ptr<material_description> description,
        std::shared_ptr<shaders::light_shader> shader);
    ~light_material_builder();

    std::string const& name() const;

    void build(parameter_collection const& parameter_overrides,
        parameter_collection const& shared_parameters,
        graphics_device& device);

    std::shared_ptr<light_material> built_material();

private: // data member
    std::shared_ptr<material_description> m_description;
    std::shared_ptr<shaders::light_shader> m_shader;
    std::shared_ptr<light_material> m_light_material;
};

// Free functions

material_builder create_material_builder(config_node const& config,
    shaders::surface_shader_collection& shaders);

light_material_builder create_light_material_builder(config_node const& config,
    shaders::light_shader_collection& shaders);

}}}} // namespace pt::d3d9::rendering::source

#endif
