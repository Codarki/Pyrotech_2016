#ifndef PT_D3D9_RENDERING_GEOMETRY_BUILDER_H
#define PT_D3D9_RENDERING_GEOMETRY_BUILDER_H

#include "../device_child.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"
#include "pt/std/map.h"

namespace p3d_format {
    class item;
    class mesh;
    class triangular_mesh;
    class serialized_mesh;
}

namespace pt {
    class parameter_collection;
}

namespace pt { namespace d3d9 {
    class stream_source;
}}

namespace pt { namespace d3d9 { namespace rendering {

class geometry;
class material_cache;

typedef std::map<std::string,std::shared_ptr<rendering::geometry> >
    topology_cache;

class geometry_builder : device_reference
{
public:
    geometry_builder(graphics_device& device);

    void set_source(p3d_format::mesh const& mesh);
    void set_serialized_source(p3d_format::serialized_mesh const& mesh);
    void set_parameters(parameter_collection const& parameters);
    void set_material_cache(material_cache& materials);
    void set_topology_cache(topology_cache& topologies);

    void set_vertex_stream_filename(std::string const& name);

    std::shared_ptr<rendering::geometry> shared_geometry();

private: // helper functions
    std::shared_ptr<rendering::geometry> build();
    std::shared_ptr<stream_source> create();
    std::shared_ptr<stream_source> load();

private: // data members
    //shared_ptr<rendering::geometry> m_cached;
    p3d_format::mesh const* m_source;
    p3d_format::serialized_mesh const* m_serialized_source;
    parameter_collection const* m_parameters;
    topology_cache* m_topologies;
    material_cache* m_materials;

    std::string m_vertex_stream_filename;
    std::vector<size_t> m_indices;
};

}}} // namespace pt::d3d9::rendering

#endif
