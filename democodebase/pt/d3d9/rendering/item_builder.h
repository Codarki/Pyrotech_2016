#ifndef PT_D3D9_RENDERING_ITEM_BUILDER_H
#define PT_D3D9_RENDERING_ITEM_BUILDER_H

#include "pt/d3d9/device_child.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/matrix.h"
#include "pt/path_fwd.h"
#include "geometry_builder.h"
#include "projection_matrix.h"

namespace p3d_format {
    class camera;
    class item;
    class light;
    class mesh;
    class serialized_mesh;
}

namespace pt {
    class config_node;
    class parameter_collection;
    class progress_report;
}

namespace pt { namespace d3d9 { namespace rendering {

class item;
class material_cache;

class item_builder
{
public:
    item_builder(graphics_device& device,
        parameter_collection const& parameters, progress_report* progress);

    void build(path const& path, material_cache& materials,
        topology_cache& topologies);
    void build(p3d_format::camera const& camera, std::shared_ptr<pt::d3d9::rendering::projection_matrix> projection);
    void build(p3d_format::light const& light, config_node light_config, material_cache& materials);
    void build(p3d_format::mesh const& mesh, material_cache& materials, topology_cache& topologies);
    void build(p3d_format::serialized_mesh const& mesh, material_cache& materials);

    std::shared_ptr<item> built_item() const;

private: // noncopyable
    item_builder(item_builder const&);
    item_builder const& operator=(item_builder const&);

private: // data members
    graphics_device* m_device;
	std::shared_ptr<item> m_item;
    parameter_collection const* m_parameters;
    progress_report* m_progress;
};

parameter_collection create_camera_parameters(
    p3d_format::camera const& camera);

}}} // namespace pt::d3d9::rendering

#endif
