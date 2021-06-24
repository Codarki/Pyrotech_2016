#ifndef PT_D3D9_RENDERING_SOURCE_MATERIAL_COLLECTION_H
#define PT_D3D9_RENDERING_SOURCE_MATERIAL_COLLECTION_H

#include "material_builder.h"
#include "pt/d3d9/shaders/light_shader_collection.h"
#include "pt/d3d9/shaders/surface_shader_collection.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/path_fwd.h"
#include "pt/std/list.h"
#include "pt/std/memory.h"

namespace pt {
    class parameter_collection;
}

namespace pt { namespace d3d9 { namespace rendering { namespace source {

class material;

class material_collection
{
public:
    void add(pt::path const& path);

    std::shared_ptr<material const> get(std::string const& name) const;

    std::shared_ptr<material> populate(parameter_collection const& p3d_parameters,
        parameter_collection const& shared_parameters,
        graphics_device& device);

    std::shared_ptr<material> populate(std::string const& name,
        parameter_collection const& parameters,
        parameter_collection const& shared_parameters,
        graphics_device& device);

    std::shared_ptr<light_material> populate_light(
        parameter_collection const& p3d_parameters,
        parameter_collection const& shared_parameters,
        graphics_device& device);

private: // helper functions
    material_builder* get_builder_by_p3d_name(std::string const& name);
    material_builder* get_builder_by_name(std::string const& name);
    light_material_builder* get_light_builder_by_name(std::string const& name);
    std::shared_ptr<material> try_get(std::string const& name);

private: // data members
    std::list<std::shared_ptr<material>> m_materials;
    shaders::surface_shader_collection m_shaders;
    std::list<material_builder> m_builders;
    shaders::light_shader_collection m_light_shaders;
    std::list<light_material_builder> m_light_builders;
    std::list<std::shared_ptr<light_material> > m_light_materials;
};

/// Returns all materials under \a path.
std::unique_ptr<material_collection> load_materials(pt::path const& path);

}}}} // namespace pt::d3d9::rendering::source

#endif
