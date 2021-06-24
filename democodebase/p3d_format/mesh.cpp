#include "mesh.h"
#include "point.h"
#include "polygon.h"
#include "chunk_reader.h"
#include "id.h"
#include "version.h"
#include "pt/any_error.h"
#include "pt/matrix_utility.h"
#include "pt/progress_report.h"
#include "pt/vector_utility.h"
#include "pt/log.h"
#include "pt/format.h"

namespace p3d_format {

// mesh_reference

mesh_reference::mesh_reference(chunk_reader& chunk)
{
    assert(chunk.id() == id_mesh);

    m_name = chunk.read<string_wrapper>().string();
    m_filename = chunk.read<string_wrapper>().string();
    m_matrix = chunk.read<matrix4x4f>();

    uint32 material_count = chunk.read<uint32>();
    if (material_count > 0)
    {
        m_material = chunk.read<string_wrapper>().string();
    }

    vector4f row_0 = m_matrix.row(0);
    vector4f row_1 = m_matrix.row(1);
    vector4f row_2 = m_matrix.row(2);

    float scale_x = length(vector3f(row_0[0], row_0[1], row_0[2]));
    float scale_y = length(vector3f(row_1[0], row_1[1], row_1[2]));
    float scale_z = length(vector3f(row_2[0], row_2[1], row_2[2]));

    if (std::abs(scale_x - scale_y) > 0.3
        || std::abs(scale_x - scale_z) > 0.3)
    {
        log(format("%1% non-uniform scale: %2% %3% %4%")
            % m_name % scale_x % scale_y % scale_z);
    }
}

std::string const& mesh_reference::name() const
{
    return m_name;
}

std::string const& mesh_reference::filename() const
{
    return m_filename;
}

std::string const& mesh_reference::material() const
{
    return m_material;
}

matrix4x4f const& mesh_reference::matrix() const
{
    return m_matrix;
}

// serialized_mesh(mesh_reference const& mesh_reference);

serialized_mesh::serialized_mesh(mesh_reference const& mesh_reference)
: m_reference(mesh_reference)
{
    this->set_name(m_reference.name());
    m_matrix = m_reference.matrix();

    if (!m_reference.material().empty())
    {
        face_material face_mat(m_reference.material());
        m_materials.push_back(face_mat);
    }
}

matrix4x4f const& serialized_mesh::default_pose() const
{
    return m_matrix;
}

std::string const& serialized_mesh::filename() const
{
    return m_reference.filename();
}

std::vector<face_material> const& serialized_mesh::materials() const
{
    return m_materials;
}


// face_material

face_material::face_material(std::string const& material_name,
    size_t reserve)
:   m_material_name(material_name)
{
    m_face_indices.reserve(reserve);
}

void face_material::add(uint16 face_index)
{
    m_face_indices.push_back(face_index);
}

void face_material::reserve(size_t count)
{
    m_face_indices.reserve(count);
}

std::string const& face_material::name() const
{
    return m_material_name;
}

std::vector<uint16> const& face_material::indices() const
{
    return m_face_indices;
}

// mesh

mesh::mesh(chunk_reader& chunk, std::string const& filename,
    progress_report* progress)
:   m_chunk(chunk)
,   m_filename(filename)
,   m_progress(progress)
{
    if (m_chunk.id() != id_mesh_model)
        throw any_error("not a .p3d_model");

    internalize();

    auto result = m_topology.populate(*this, m_progress);

    // Verbose logging.
    if (result.collapsed_triangle_count > 0)
    {
        log(pt::format("ignoring %1% collapsed polygons. (out of %2% triangles)")
            % result.collapsed_triangle_count
            % result.triangle_count);
    }

    if (result.ignored_triangles_because_surface_area_is_below_epsilon > 0)
    {
        log(pt::format("ignoring %1% polygons with surface area less than 0.000001. (out of %2% triangles)")
            % result.ignored_triangles_because_surface_area_is_below_epsilon
            % result.triangle_count);
    }

    if (result.failed_tangent_calculation_count > 0)
    {
        log(pt::format("failed to calculate tangent for %1% polygons. (out of %2% triangles)")
            % result.failed_tangent_calculation_count
            % result.triangle_count);
    }
}

void mesh::set_default_pose(matrix4x4f const& matrix)
{
    m_matrix = matrix;
}

matrix4x4f const& mesh::default_pose() const
{
    return m_matrix;
}

std::string const& mesh::filename() const
{
    return m_filename;
}

//std::string const& mesh::model_name() const
//{
//    return m_model_name;
//}

std::vector<vector3f> const& mesh::position() const
{
    return m_positions;
}

std::vector<vector2f> const& mesh::texcoord() const
{
    return m_texcoord;
}

std::vector<vector4i> const& mesh::indices() const
{
    return m_indices;
}

std::vector<face_material> const& mesh::materials() const
{
    return m_materials;
}

std::vector<uint32> const& mesh::smoothing_groups() const
{
    return m_smoothing_groups;
}

p3d_format::topology const& mesh::topology() const
{
    return m_topology;
}

vector3f read_position(chunk_reader& chunk, float radius)
{
    uint16 values[3];
    chunk.read_array<uint16>(3, values);

    // 32768 is 0.0f
    float x = (values[0] - 32768) / 32767.0f;
    float y = (values[1] - 32768) / 32767.0f;
    float z = (values[2] - 32768) / 32767.0f;

    return vector3f(x, y, z) * radius;
}

void mesh::internalize()
{
    check_version();

    // Read general info.
    set_name(m_chunk.read<string_wrapper>());

    size_t vertex_count = m_chunk.read<uint32>();
    size_t face_count = m_chunk.read<uint32>();
    size_t material_count = m_chunk.read<uint32>();

    // Read vertices, faces etc..
    m_positions.reserve(vertex_count);
    m_texcoord.reserve(vertex_count);

    float radius = m_chunk.read<float>();

    for (size_t i = 0; i < vertex_count; ++i)
    {
        //m_positions.push_back(m_chunk.read<vector3f>());
        m_positions.push_back(read_position(m_chunk, radius));
        m_texcoord.push_back(m_chunk.read<vector2f>());
    }
    if (m_progress)
        m_progress->advance();

    m_indices.reserve(face_count);

    if (vertex_count > 255)
    {
        for (size_t i = 0; i < face_count; ++i)
        {
            typedef vector<unsigned short, 4> face_type;
            face_type face = m_chunk.read<face_type>();

            vector4i ar = static_vector_cast<int>(face);

            m_indices.push_back(ar);
        }
    }
    else
    {
        for (size_t i = 0; i < face_count; ++i)
        {
            typedef vector<unsigned char, 4> face_type;
            face_type face = m_chunk.read<face_type>();

            vector4i ar = static_vector_cast<int>(face);

            m_indices.push_back(ar);
        }
    }
    if (m_progress)
        m_progress->advance();

    bool per_face_materials = m_chunk.read<bool>();
    for (size_t i = 0; i < material_count; ++i)
    {
        face_material face_mat(m_chunk.read<string_wrapper>().string());

        if (per_face_materials)
        {
            uint32 face_index_count = m_chunk.read<uint32>();
            face_mat.reserve(face_index_count);

            for (size_t j = 0; j < face_index_count; ++j)
                face_mat.add(m_chunk.read<uint16>());
        }

        m_materials.push_back(face_mat);
    }

    bool per_face_smoothing_groups = m_chunk.read<bool>();
    if (per_face_smoothing_groups)
    {
        m_smoothing_groups.reserve(face_count);

        for (size_t i = 0; i < face_count; ++i)
        {
            uint32 sg = m_chunk.read<uint32>();
            if (sg == 0)
                sg = 1U << 31;
            m_smoothing_groups.push_back(sg);
        }
    }
    if (m_progress)
        m_progress->advance();
}

void mesh::check_version()
{
    uint16 version = m_chunk.read<uint16>();

    if (version != p3d_format::version_model)
    {
        std::string error = (format("Model version %1%.%2%, expected %3%.%4%")
            % PT_VERSION_MAJOR(version)
            % PT_VERSION_MINOR(version)
            % PT_VERSION_MAJOR(p3d_format::version_model)
            % PT_VERSION_MINOR(p3d_format::version_model)).move_string();

        throw std::exception(error.c_str());
    }

    for (size_t i = 0; i < 22; ++i)
    {
        uint8 crap = m_chunk.read<uint8>();
        crap;
    }
}

// unoptimized_triangular_mesh

unoptimized_triangular_mesh::unoptimized_triangular_mesh(
    topology const& topology)
{
    for (std::vector<corner_access>::const_iterator i
        = topology.corners().begin(); i != topology.corners().end(); ++i)
    {
    }

    size_t corner_index = 0;

    for (std::vector<polygon>::const_iterator i
        = topology.polygons().begin(); i != topology.polygons().end(); ++i)
    {
        polygon const& polygon = *i;

        for (size_t c = 0; c < polygon.corner_count(); ++c)
        {
            push_back_vertex(polygon.corner_at(c), topology);
            m_indices.push_back(corner_index);
            ++corner_index;
        }
    }
}

std::vector<vector3f> const& unoptimized_triangular_mesh::positions() const
{
    return m_positions;
}

std::vector<vector3f> const& unoptimized_triangular_mesh::vertex_normals() const
{
    return m_vertex_normals;
}

std::vector<vector4f> const& unoptimized_triangular_mesh::vertex_tangents() const
{
    return m_vertex_tangents;
}

//std::vector<vector3f> const& unoptimized_triangular_mesh::vertex_bitangents() const
//{
//    return m_vertex_bitangents;
//}

std::vector<vector2f> const& unoptimized_triangular_mesh::texcoords() const
{
    return m_texcoords;
}

std::vector<size_t> const& unoptimized_triangular_mesh::indices() const
{
    return m_indices;
}

void unoptimized_triangular_mesh::push_back_vertex(
    corner_access const& corner, topology const& topology)
{
    point const* point = corner.get().point();
    vector3f position = topology.attributes().position.value_for(point);

    corner_data const& data = topology.attributes().corner.value_for(corner);
    //vector3f texcoord = data.texcoords[0];

    //vector3f normal = topology.attributes().corner_normal.value_for(corner);
    //vector4f tangent = topology.attributes().corner_tangent.value_for(corner);
    ////vector3f bitangent = topology.attributes().point_bitangent.value_for(point);
    vector3f texcoord = topology.attributes().texcoords[0].value_for(corner);

    m_positions.push_back(position);
    m_vertex_normals.push_back(data.normal);
    m_vertex_tangents.push_back(data.tangent);
    //m_vertex_bitangents.push_back(bitangent);
    m_texcoords.push_back(vector2f(texcoord[0], texcoord[1]));
}

} // namespace p3d_format
