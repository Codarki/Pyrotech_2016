#ifndef PT_D3D9_RENDERING_MATERIAL_CACHE_H
#define PT_D3D9_RENDERING_MATERIAL_CACHE_H

#include "material_cache_fwd.h"
#include "light_properties_cache.h"
#include "surface_cache.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/path.h"
#include "pt/std/memory.h"

namespace pt {
    class parameter_collection;
    namespace d3d9 { namespace rendering { namespace source {
        class material_collection;
    }}}
}

namespace pt { namespace d3d9 { namespace rendering {

// TODO: WTF is this class. Explain, clearly.
/// Owned by demo_player. Hybrid loading of materials, creating of surfaces and surface cache.
class material_cache
{
public:
    material_cache(parameter_collection& shared_parameters);
    ~material_cache();

    std::string parameters_as_readable_text() const;
    parameter_collection& shared_parameters();

    void load_materials(pt::path const& path);

	std::shared_ptr<surface> find_or_create(graphics_device& device,
        parameter_collection const& parameters);
    std::shared_ptr<surface> find_or_create(graphics_device& device,
        std::string const& name, parameter_collection const& parameters);

    std::shared_ptr<light_properties> find_or_create_light(
        graphics_device& device,
        parameter_collection const& parameters);

private: // data members
    std::unique_ptr<source::material_collection> m_materials;
    surface_cache m_surfaces;
    light_properties_cache m_light_properties;
    parameter_collection* m_shared_parameters;
};

}}} // namespace pt::d3d9::rendering

#endif
