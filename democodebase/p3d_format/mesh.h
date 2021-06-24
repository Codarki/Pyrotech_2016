#ifndef PT_P3D_FORMAT_MESH_H
#define PT_P3D_FORMAT_MESH_H

#include "item.h"
#include "pt/boost/noncopyable.h"
#include "topology.h"
#include "pt/basic_types.h"
#include "pt/matrix.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"

namespace pt {
class progress_report;
}

namespace p3d_format {

class chunk_reader;

class mesh_reference
{
public:
    mesh_reference(chunk_reader& chunk);

    std::string const& name() const;
    std::string const& filename() const;
    std::string const& material() const;
    matrix4x4f const& matrix() const;

private:
    std::string m_name;
    std::string m_filename;
    matrix4x4f m_matrix;
    std::string m_material;
};

class face_material
{
public:
    face_material(std::string const& material_name, size_t reserve = 0);
    void add(uint16 face_index);

    void reserve(size_t count);

    std::string const& name() const;
    std::vector<uint16> const& indices() const;

private: // data members
    std::string m_material_name;
    std::vector<uint16> m_face_indices;
};

class serialized_mesh : public item
{
public:
    serialized_mesh(mesh_reference const& mesh_reference);

    void set_default_pose(matrix4x4f const& matrix);
    matrix4x4f const& default_pose() const;

    std::string const& filename() const;

    std::vector<face_material> const& materials() const;

private: // data members
    mesh_reference m_reference;
    matrix4x4f m_matrix;
    std::vector<face_material> m_materials;
};

class mesh : public item, boost::noncopyable
{
public:
    mesh(chunk_reader& chunk, std::string const& filename,
        progress_report* progress);

    void set_default_pose(matrix4x4f const& matrix);
    matrix4x4f const& default_pose() const;

    std::string const& filename() const;

    //std::string const& model_name() const;
    std::vector<vector3f> const& position() const;
    std::vector<vector2f> const& texcoord() const;
    std::vector<vector4i> const& indices() const;
    std::vector<face_material> const& materials() const;
    std::vector<uint32> const& smoothing_groups() const;

    p3d_format::topology const& topology() const;

private: // helper functions
    void internalize();
    void check_version();

private: // data members
    chunk_reader& m_chunk;
    std::string m_filename;
    progress_report* m_progress;

    //std::string m_model_name;
    std::vector<vector3f> m_positions;
    std::vector<vector2f> m_texcoord;
    std::vector<vector4i> m_indices;
    std::vector<face_material> m_materials;
    std::vector<uint32> m_smoothing_groups;
    matrix4x4f m_matrix;

    p3d_format::topology m_topology;
};

class unoptimized_triangular_mesh
{
public:
    unoptimized_triangular_mesh(topology const& topology);

    std::vector<vector3f> const& positions() const;
    std::vector<vector3f> const& vertex_normals() const;
    std::vector<vector4f> const& vertex_tangents() const;
    //std::vector<vector3f> const& vertex_bitangents() const;
    std::vector<vector2f> const& texcoords() const;
    std::vector<size_t> const& indices() const;

private: // helper functions
    void push_back_vertex(corner_access const& corner, topology const& topology);

private:
    std::vector<vector3f> m_positions;
    std::vector<vector3f> m_vertex_normals;
    std::vector<vector4f> m_vertex_tangents;
    //std::vector<vector3f> m_vertex_bitangents;
    std::vector<vector2f> m_texcoords;
    std::vector<size_t> m_indices;
};

} // namespace p3d_format

#endif
