#include "geometry_builder.h"
#include "drawable_builder.h"
#include "attribute_collection.h"
#include "basic_geometry.h"
#include "material_cache.h"
#include "static_vertex_stream.h"
#include "serialized_mesh_not_found_error.h"
#include "source/vertex_attribute.h"
#include "pt/d3d9/vertex_streams.h"
#include "p3d_format/mesh.h"
#include "p3d_format/triangular_mesh.h"
#include "pt/log.h"
#include "pt/string_wrapper.h"
#include "pt/parameter_collection.h"
#include "pt/file_input_stream.h"
#include "pt/file_output_stream.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/boost/cast.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    void write_index_buffer_dump(
        std::vector<size_t> const& indices, output_stream& output)
    {
        std::vector<uint32> tmp_indices(indices.size());

        for (size_t i = 0; i < indices.size(); ++i)
            tmp_indices[i] = boost::numeric_cast<uint32>(indices[i]);

        pt::write(output, uint32(indices.size()));
        output.write(&indices[0], tmp_indices.size() * sizeof(uint32));
    }

    std::vector<size_t> read_index_buffer_dump(input_stream& input)
    {
        size_t count = pt::read<uint32>(input);
        std::vector<uint32> indices(count);
        input.read(&indices[0], count * sizeof(uint32));

        std::vector<size_t> result(count);
        for (size_t i = 0; i < count; ++i)
            result[i] = indices[i];

        return result;
    }
}

geometry_builder::geometry_builder(graphics_device& device)
:   device_reference(device)
,   m_source(0)
,   m_serialized_source(0)
,   m_parameters(0)
,   m_materials(0)
{
}

void geometry_builder::set_source(p3d_format::mesh const& mesh)
{
    m_source = &mesh;
}

void geometry_builder::set_serialized_source(p3d_format::serialized_mesh const& mesh)
{
    m_serialized_source = &mesh;
}

void geometry_builder::set_parameters(parameter_collection const& parameters)
{
    m_parameters = &parameters;
}

void geometry_builder::set_material_cache(material_cache& materials)
{
    m_materials = &materials;
}

void geometry_builder::set_topology_cache(topology_cache& topologies)
{
    m_topologies = &topologies;
}

void geometry_builder::set_vertex_stream_filename(std::string const& name)
{
    m_vertex_stream_filename = name;
}

std::shared_ptr<rendering::geometry> geometry_builder::shared_geometry()
{
    if (m_source)
    {
        log(string_wrapper("Building mesh model item \"" + m_source->name() + "\""));
    }
    else
    {
        log(string_wrapper("Serializing mesh model item \"" + m_serialized_source->name() + "\""));
    }
    logging::scoped_indent indent;

    return build();
}

std::shared_ptr<rendering::geometry> geometry_builder::build()
{
    if (m_serialized_source)
    {
        assert(m_parameters && m_materials);

        std::shared_ptr<stream_source> source = load();

		std::shared_ptr<vertex_streams> streams(new vertex_streams(device()));
        streams->add(source);

        // Find materials.
        parameter_collection p3d_parameters;

        for (std::vector<p3d_format::face_material>::const_iterator i
            = m_serialized_source->materials().begin(); i != m_serialized_source->materials().end();
            ++i)
        {
            p3d_format::face_material const& face_mat = *i;

            std::shared_ptr<parameter_node> node
                = find_child("p3d material", face_mat.name(), *m_parameters);

            if (node)
            {
                p3d_parameters.add(node);
            }
            else
            {
                log("Couldnt find parameters for material " + face_mat.name());
            }
        }

        if (p3d_parameters.size() > 1)
            log("Multimaterial mesh not supported yet. First material used.");

        auto drawable = build_shared_basic_drawable(device(), streams, m_indices);
        std::shared_ptr<surface> surface
            = m_materials->find_or_create(device(), p3d_parameters);

        auto geometry = std::make_shared<basic_geometry>(drawable, surface);

        return geometry;

    }
    else
    {
        assert(m_source);
        assert(m_parameters && m_materials);

        topology_cache::const_iterator topology_iter
            = m_topologies->find(m_source->filename());

        if (topology_iter != m_topologies->end())
        {
            return topology_iter->second;
        }

        std::shared_ptr<stream_source> source = create();

        auto streams = std::make_shared<vertex_streams>(device());
        streams->add(source);

        // Find materials.
        parameter_collection p3d_parameters;
        for (std::vector<p3d_format::face_material>::const_iterator i
            = m_source->materials().begin(); i != m_source->materials().end();
            ++i)
        {
            p3d_format::face_material const& face_mat = *i;
            std::shared_ptr<parameter_node> node
                = find_child("p3d material", face_mat.name(), *m_parameters);

            if (node)
            {
                p3d_parameters.add(node);
            }
            else
            {
                log("Couldnt find parameters for material " + face_mat.name());
            }
        }

        if (p3d_parameters.size() > 1)
            log("Multimaterial mesh not supported yet. First material used.");

		std::shared_ptr<surface> surface
            = m_materials->find_or_create(device(), p3d_parameters);

        // Create geometry.
        auto geometry = std::make_shared<basic_geometry>(
            build_shared_basic_drawable(device(), streams, m_indices), surface);

        m_topologies->insert(
            topology_cache::value_type(m_source->filename(), geometry));

        return geometry;
    }
}

std::shared_ptr<stream_source> geometry_builder::create()
{
    p3d_format::triangular_mesh tri_mesh(m_source->topology());

    // Create source vertex attribute maps.
	std::shared_ptr<source::vectorf_attribute<3> > positions(
        new source::vectorf_attribute<3>(tri_mesh.positions()));
    std::shared_ptr<source::vectorf_attribute<3> > normals(
        new source::vectorf_attribute<3>(tri_mesh.vertex_normals()));
    std::shared_ptr<source::vectorf_attribute<4> > tangents(
        new source::vectorf_attribute<4>(tri_mesh.vertex_tangents()));
    //shared_ptr<source::vectorf_attribute<3> > bitangents(
    //    new source::vectorf_attribute<3>(tri_mesh.vertex_bitangents()));
    std::shared_ptr<source::vectorf_attribute<2> > texcoords(
        new source::vectorf_attribute<2>(tri_mesh.texcoords()));

    attribute_collection vertex_attributes;
    vertex_attributes.add(semantic("POSITION"), positions);
    vertex_attributes.add(semantic("TANGENT"), tangents);
    //vertex_attributes.add(semantic("BINORMAL"), bitangents);
    vertex_attributes.add(semantic("NORMAL"), normals);
    vertex_attributes.add(semantic("TEXCOORD"), texcoords);

    if (!m_vertex_stream_filename.empty())
    {
        file_output_stream file_stream(m_vertex_stream_filename);

        write_static_vertex_stream_dump(vertex_attributes, file_stream);
        write_index_buffer_dump(tri_mesh.indices(), file_stream);
    }

    // Create vertex buffer.
    std::shared_ptr<static_vertex_stream> source;
    source.reset(new static_vertex_stream(device(), vertex_attributes));

    m_indices = tri_mesh.indices();

    return source;
}

std::shared_ptr<stream_source> geometry_builder::load()
{
    if (m_vertex_stream_filename.empty())
        throw pt::any_error("m_vertex_stream_filename.empty()");

    if (!immutable_file_exists(m_vertex_stream_filename))
        throw serialized_mesh_not_found_error(m_vertex_stream_filename);

    file_input_stream file_stream(m_vertex_stream_filename);

    // Load vertex buffer.
    std::shared_ptr<static_vertex_stream> source
        = read_static_vertex_stream_dump(device(), file_stream);

    m_indices = read_index_buffer_dump(file_stream);

    return source;
}

}}} // namespace pt::d3d9::rendering
