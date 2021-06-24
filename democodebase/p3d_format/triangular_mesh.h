#ifndef PT_P3D_FORMAT_TRIANGULAR_MESH_H
#define PT_P3D_FORMAT_TRIANGULAR_MESH_H

#include "pt/vector.h"
#include "pt/std/vector.h"

namespace p3d_format {

using namespace pt;

class corner_access;
class topology;

class triangular_mesh
{
public:
    triangular_mesh(topology const& topology);

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
