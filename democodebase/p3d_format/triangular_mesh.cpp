#include "triangular_mesh.h"
#include "point.h"
#include "polygon.h"
#include "topology.h"
#include "pt/boost/noncopyable.h"
#include "pt/std/map.h"
#include "pt/any_error.h"

namespace p3d_format {

class vertex_generator : boost::noncopyable
{
public:
    vertex_generator(topology const& topology);
    std::vector<size_t> const& triangle_corner_indices() const;
    std::vector<corner_access> const& corners() const;

private: // typedefs
    typedef std::vector<corner_access> corner_container;
    typedef std::map<point const*,corner_container> vertex_container;
    typedef std::map<corner_access,size_t> corner_indices;

private: // helper functions
    corner_access const* find_equal_corner(corner_access const& corner)
        const;
    bool are_corners_equal(corner_access const& corner_0,
        corner_access const& corner_1) const;
    void insert_new_corner(corner_access const& corner);

private: // data members
    vertex_container m_vertices;
    corner_indices m_corner_indices;
    std::vector<size_t> m_triangle_indices;
    topology const& m_topology;
    corner_container m_final_corners;

    size_t m_identical_corner_count;
};

vertex_generator::vertex_generator(topology const& topology)
: m_topology(topology)
, m_identical_corner_count(0)
{
    std::vector<polygon> const& polygons = topology.polygons();

    m_final_corners.reserve(polygons.size() * 3);
    m_triangle_indices.reserve(polygons.size() * 3);

    for (std::vector<polygon>::const_iterator i = polygons.begin();
        i != polygons.end(); ++i)
    {
        polygon const& polygon = *i;

        assert(polygon.corner_count() == 3);

        for (size_t c = 0; c < polygon.corner_count(); ++c)
        {
            corner_access const& corner_0 = polygon.corner_at(c);

            size_t corner_index;

            if (corner_access const* corner_1 = find_equal_corner(corner_0))
            {
                ++m_identical_corner_count;
                corner_index = m_corner_indices.find(*corner_1)->second;
            }
            else
            {
                corner_index = m_corner_indices.size();
                
                assert(corner_index < 65536);

                m_corner_indices.insert(
                    corner_indices::value_type(corner_0, corner_index));

                insert_new_corner(corner_0);
                m_final_corners.push_back(corner_0);
            }

            m_triangle_indices.push_back(corner_index);

            //push_back_vertex(polygon->corner_at(c).get(), topology);
            //m_indices.push_back(corner_index);
            //++corner_index;
        }
    }
}

std::vector<size_t> const& vertex_generator::triangle_corner_indices() const
{
    return m_triangle_indices;
}

std::vector<corner_access> const& vertex_generator::corners() const
{
    return m_final_corners;
}

corner_access const* vertex_generator::find_equal_corner(
    corner_access const& corner) const
{
    point const* point = corner.get().point();

    vertex_container::const_iterator i = m_vertices.find(point);
    if (i == m_vertices.end())
        return 0;

    corner_container const& corners = i->second;
    for (corner_container::const_iterator j = corners.begin();
        j != corners.end(); ++j)
    {
        if (are_corners_equal(corner, *j))
        {
            return &*j;
        }
    }
    return 0;
}

bool vertex_generator::are_corners_equal(corner_access const& corner_0,
    corner_access const& corner_1) const
{
    return p3d_format::are_corners_equal(corner_0, corner_1, m_topology);
}

void vertex_generator::insert_new_corner(corner_access const& corner)
{
    point const* point = corner.get().point();
    vertex_container::iterator i = m_vertices.find(point);
    if (i == m_vertices.end())
    {
        corner_container corners;
        corners.reserve(8); // Normally 4-8 triangles share a vertex.
        corners.push_back(corner);

        if (!m_vertices.insert(vertex_container::value_type(point, corners)).second)
        {
            throw any_error("merging vertices failed");
        }
        return;
    }

    i->second.push_back(corner);
}

// triangular_mesh

triangular_mesh::triangular_mesh(topology const& topology)
{
    vertex_generator generator(topology);

    m_indices = generator.triangle_corner_indices();

    std::vector<corner_access> const& corners = generator.corners();

    m_positions.reserve(corners.size());
    m_vertex_normals.reserve(corners.size());
    m_vertex_tangents.reserve(corners.size());
    m_texcoords.reserve(corners.size());

    for (std::vector<corner_access>::const_iterator i = corners.begin();
        i != corners.end(); ++i)
    {
        push_back_vertex(*i, topology);
    }
}

std::vector<vector3f> const& triangular_mesh::positions() const
{
    return m_positions;
}

std::vector<vector3f> const& triangular_mesh::vertex_normals() const
{
    return m_vertex_normals;
}

std::vector<vector4f> const& triangular_mesh::vertex_tangents() const
{
    return m_vertex_tangents;
}

std::vector<vector2f> const& triangular_mesh::texcoords() const
{
    return m_texcoords;
}

std::vector<size_t> const& triangular_mesh::indices() const
{
    return m_indices;
}

void triangular_mesh::push_back_vertex(corner_access const& corner,
    topology const& topology)
{
    point const* point = corner.get().point();
    vector3f position = topology.attributes().position.value_for(point);

    corner_data const& data = topology.attributes().corner.value_for(corner);

    //vector3f normal = data.normal;
    //vector4f tangent = data.tangent;
    //vector3f texcoord = !data.texcoords.empty() ? data.texcoords[0] : vector3f(0, 0, 0);

    //vector3f normal = topology.attributes().corner_normal.value_for(corner);
    //vector4f tangent = topology.attributes().corner_tangent.value_for(corner);
    vector3f texcoord = topology.attributes().texcoords[0].value_for(corner);

    m_positions.push_back(position);
    m_vertex_normals.push_back(data.normal);
    m_vertex_tangents.push_back(data.tangent);
    //m_vertex_bitangents.push_back(bitangent);
    m_texcoords.push_back(vector2f(texcoord[0], texcoord[1]));
}

} // namespace p3d_format
