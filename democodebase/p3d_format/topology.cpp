#include "topology.h"
#include "mesh.h"
#include "point.h"
#include "polygon.h"
#include "pt/any_error.h"
#include "pt/debug_only.h"
#include "pt/log.h"
#include "pt/none.h"
#include "pt/progress_report.h"
#include "pt/format.h"
#include "pt/string_builder.h"
#include "pt/std/map.h"

namespace p3d_format {

namespace {
    vector3f face_normal(vector3f const& v0, vector3f const& v1,
        vector3f const& v2)
    {
        vector3f edge0 = v1 - v0;
        vector3f edge1 = v2 - v0;
        
        if (length(edge0) < 0.000001f)
        {
            edge0 = v1 - v2;
            assert(false);
        }
        if (length(edge1) < 0.000001f)
        {
            edge1 = v2 - v1;
            assert(false);
        }

        vector3f x = pt::cross(edge0, edge1);

        return normalize(x);
    }

    float surface_area(vector3f v[3])
    {
        vector3f edge0 = v[1] - v[0];
        vector3f edge1 = v[2] - v[0];

        return (length(edge0) * length(edge1)) / 2.0f;
    }

    void calculate_polygon_tangent(vector3f v[3], vector2d texcoord[3],
        vector3f& tangent, vector3f& bitangent, vector3f const&,
        bool& failed)
    {
        //vector3d edge0 = static_vector_cast<double>(v[1] - v[0]);
        //vector3d edge1 = static_vector_cast<double>(v[2] - v[0]);
        vector3d edge0 = static_vector_cast<double>(v[1]) - static_vector_cast<double>(v[0]);
        vector3d edge1 = static_vector_cast<double>(v[2]) - static_vector_cast<double>(v[0]);

        vector2d uv0 = texcoord[1] - texcoord[0];
        vector2d uv1 = texcoord[2] - texcoord[0];

        double epsilon = 0.000001; //std::numeric_limits<double>::epsilon();

        double det = uv0[0] * uv1[1] - uv1[0] * uv0[1];
        if (std::abs(det) < epsilon)
        {
            // Prevent Divide by zero
            //det = 1.0f;

            //std::string debug_text("polygon " + pt::to_string(polygon_index));
            //log(debug_text
            //    + ": Failed to calculate tangent space from UV coordinates.");

            failed = true;
            return;
        }
        double r = 1.0 / det;

        vector3d sdir = (edge0 * uv1[1] - edge1 * uv0[1]) * r;
        vector3d tdir = (edge1 * uv0[0] - edge0 * uv1[0]) * r;

        tangent = static_vector_cast<float>(sdir);
        bitangent = static_vector_cast<float>(tdir);
    }

    template<int C>
    bool within_epsilon(vector<float,C> const& a, vector<float,C> const& b)
    {
        float epsilon = 0.0001f;
        for (size_t i = 0; i < C; ++i)
        {
            float difference = a[i] - b[i];

            if (difference > epsilon)
                return false;

            if (difference < -epsilon)
                return false;
        }
        return true;
    }
} // anonymous namespace

class topology_positions
{
public:
    topology_positions(mesh const& mesh)
    {
        size_t combined_count = 0;
        size_t original_point_count = mesh.position().size();
        //size_t polygon_count = mesh.indices().size();

        m_original_positions.resize(original_point_count);
        m_combined_positions.resize(original_point_count);

        // Create points.
        vector3f const* position_array = mesh.position().data();
        vector3f* original_position_array = m_original_positions.data();
        vector3f* combined_position_array = m_combined_positions.data();

        m_combined_position_count = 0;

        for (size_t i = 0; i < original_point_count; ++i)
        {
            //vector3f position = mesh.position()[i];
            //m_original_positions.push_back(position);

            vector3f const& position = position_array[i];
            original_position_array[i] = position;

            size_t j = find_equal_position(position);

            if (j != none<size_t>())
            {
                //m_orignal_index_to_combined[i] = j;
                m_orignal_index_to_combined.insert(std::make_pair(i, j));
                ++combined_count;
            }
            else
            {
                //size_t combined_index = m_combined_positions.size();
                size_t combined_index = m_combined_position_count;

                //m_orignal_index_to_combined[i] = combined_index;
                m_orignal_index_to_combined.insert(std::make_pair(i, combined_index));

                //m_combined_positions.push_back(position);

                combined_position_array[m_combined_position_count] = position;
                ++m_combined_position_count;
            }
        }

        m_combined_positions.resize(m_combined_position_count);

        //if (combined_count > 0)
        //{
        //    log(format("Combined %1% positions from %2%")
        //        % combined_count % original_point_count);
        //}
    }

    size_t index_for_original_index(size_t original_index) const
    {
        std::map<size_t,size_t>::const_iterator i
            = m_orignal_index_to_combined.find(original_index);
        assert(i != m_orignal_index_to_combined.end());

        return i->second;
    }

    std::vector<vector3f> const& combined() const
    {
        return m_combined_positions;
    }

private:
    size_t find_equal_position(vector3f const& position) const
    {
        vector3f const* combined_position_array = m_combined_positions.data();

        for (size_t index = 0; index < m_combined_position_count; ++index)
        {
            vector3f const& combined_position = combined_position_array[index];

            if (within_epsilon(combined_position, position))
                return index;

            ++index;
        }

        //size_t index = 0;
        //for (std::vector<vector3f>::const_iterator i
        //    = m_combined_positions.begin(), end = m_combined_positions.end();
        //    i != end && index < m_combined_posixtion_count; ++i)
        //{
        //    if (within_epsilon(*i, position))
        //        return index;

        //    ++index;
        //}

        return none<size_t>();
    }

private: // data members
    std::vector<vector3i> m_original_face_indices;
    std::map<size_t,size_t> m_orignal_index_to_combined;
    std::vector<vector3f> m_original_positions;
    std::vector<vector3f> m_combined_positions;
    size_t m_combined_position_count;
};

topology_population_result topology::populate(mesh const& mesh, progress_report* progress)
{
    log("Creating topology for " + mesh.name());
    logging::scoped_indent indent;

    topology_positions positions(mesh);

    assert(m_points.empty());

    // Set material names.
    for (std::vector<face_material>::const_iterator i
        = mesh.materials().begin(); i != mesh.materials().end(); ++i)
    {
        m_attributes.materials.push_back(i->name());
    }

    //size_t point_count = mesh.position().size();
    size_t const point_count = positions.combined().size();
    size_t const polygon_count = mesh.indices().size();

    topology_population_result result;
    result.triangle_count = polygon_count;

    // Create points.
    m_points.reserve(point_count);
    m_attributes.position.reserve(point_count);
    m_attributes.position.begin_inserting();

    for (size_t i = 0; i < point_count; ++i)
    {
        m_points.push_back(point());

        //vector3f position = mesh.position()[i];
        vector3f position = positions.combined()[i];

        m_attributes.position.add(&m_points.back(), static_cast<vector3f&&>(position)); // move
    }

    m_attributes.position.end_inserting();

    // Create polygons and corners.
    polygon_attribute<uint32> polygon_smoothing_groups;
    if (!mesh.smoothing_groups().empty())
    {
        polygon_smoothing_groups.reserve(polygon_count);
    }
    polygon_smoothing_groups.begin_inserting();

    m_polygons.reserve(polygon_count);
    m_corners.reserve(polygon_count * 3);

    m_attributes.polygon.reserve(polygon_count);
    m_attributes.polygon.begin_inserting();

    m_attributes.texcoords.resize(1);
    m_attributes.texcoords[0].reserve(polygon_count * 3);
    m_attributes.texcoords[0].begin_inserting();

    for (size_t i = 0; i < polygon_count; ++i)
    {
        // Reverse order, to make it from RH to LH system.
        // TODO make p3d to store LH natively.
        //int i0 = mesh.indices()[i][0];
        //int i1 = mesh.indices()[i][2];
        //int i2 = mesh.indices()[i][1];

        int original_index_0 = mesh.indices()[i][0];
        int original_index_1 = mesh.indices()[i][2];
        int original_index_2 = mesh.indices()[i][1];
        int i0 = static_cast<int>(positions.index_for_original_index(original_index_0));
        int i1 = static_cast<int>(positions.index_for_original_index(original_index_1));
        int i2 = static_cast<int>(positions.index_for_original_index(original_index_2));
        if (i0 == i1 || i0 == i2 || i1 == i2)
        {
            ++result.collapsed_triangle_count;
            continue;
        }

        vector3f v[3];
        //v[0] = mesh.position()[i0];
        //v[1] = mesh.position()[i1];
        //v[2] = mesh.position()[i2];
        //vector2f tex0 = mesh.texcoord()[i0];
        //vector2f tex1 = mesh.texcoord()[i1];
        //vector2f tex2 = mesh.texcoord()[i2];
        v[0] = positions.combined()[i0];
        v[1] = positions.combined()[i1];
        v[2] = positions.combined()[i2];
        vector2f tex0 = mesh.texcoord()[original_index_0];
        vector2f tex1 = mesh.texcoord()[original_index_1];
        vector2f tex2 = mesh.texcoord()[original_index_2];

        if (surface_area(v) < 0.000001f)
        {
            ++result.ignored_triangles_because_surface_area_is_below_epsilon;
            continue;
        }

        vector2d t[3];
        t[0] = static_vector_cast<double>(tex0);
        t[1] = static_vector_cast<double>(tex1);
        t[2] = static_vector_cast<double>(tex2);

        vector3f fnormal = face_normal(v[0], v[1], v[2]);

        vector3f tangent;
        vector3f bitangent;

        bool failed = false;
        
        calculate_polygon_tangent(v, t, tangent, bitangent, fnormal, failed);
        if (failed)
        {
            ++result.failed_tangent_calculation_count;
            continue;
        }

        point& p0 = m_points[i0];
        point& p1 = m_points[i1];
        point& p2 = m_points[i2];

        corner_access c0 = p0.create_corner();
        corner_access c1 = p1.create_corner();
        corner_access c2 = p2.create_corner();

        m_corners.push_back(c0);
        m_corners.push_back(c1);
        m_corners.push_back(c2);

        m_polygons.push_back(polygon());
        polygon& polygon = m_polygons.back();

        polygon_data polygon_data;

        polygon.add_corner(c0);
        polygon.add_corner(c1);
        polygon.add_corner(c2);

        m_attributes.texcoords[0].add(c0, vector3f(tex0, 0));
        m_attributes.texcoords[0].add(c1, vector3f(tex1, 0));
        m_attributes.texcoords[0].add(c2, vector3f(tex2, 0));

        polygon_data.normal = fnormal;

        polygon_data.tangent = tangent;
        polygon_data.bitangent = bitangent;

        for (std::vector<face_material>::const_iterator j
            = mesh.materials().begin(), j_end = mesh.materials().end(); j != j_end; ++j)
        {
            bool already_added = false;
            std::vector<uint16> const& indices = j->indices();

            for (std::vector<uint16>::const_iterator k = indices.begin(), k_end = indices.end();
                k != k_end; ++k)
            {
                if (*k == i)
                {
                    assert(!already_added);
                    polygon_data.material_indices
                        = static_cast<uint32>(material_index(j->name()));
                    already_added = true;
                }
            }
        }

        m_attributes.polygon.add(&polygon, polygon_data);

        if (!mesh.smoothing_groups().empty())
        {
            uint32 smoothing_group = mesh.smoothing_groups()[i];
            polygon_smoothing_groups.add(&polygon, smoothing_group);
        }

        if (progress)
            progress->advance();
    }

    polygon_smoothing_groups.end_inserting();

    m_attributes.polygon.end_inserting();
    m_attributes.texcoords[0].end_inserting();


    if (m_polygons.size() == 0)
        throw topology_building_error("No polygons.");

    // Calclulate corner normals and tangents.

    m_attributes.corner.reserve(m_polygons.size() * 3);
    m_attributes.corner.begin_inserting();

    for (size_t i = 0; i < m_polygons.size(); ++i)
    {
        polygon& polygon = m_polygons[i];

        // This actually works with N-sided polygons, but would be better to
        // triangulate first.
        assert(polygon.corner_count() == 3);

        for (size_t c = 0; c < polygon.corner_count(); ++c)
        {
            calculate_corner_normal(&polygon,
                polygon.corner_at(c), polygon_smoothing_groups);
        }
    }

    m_attributes.corner.end_inserting();
    return result;
}

std::vector<corner_access> const& topology::corners() const
{
    return m_corners;
}

std::vector<point> const& topology::points() const
{
    return m_points;
}

std::vector<polygon> const& topology::polygons() const
{
    return m_polygons;
}

p3d_format::attributes const& topology::attributes() const
{
    return m_attributes;
}

size_t topology::material_index(std::string const& material_name) const
{
    for (size_t i = 0; i < m_attributes.materials.size(); ++i)
    {
        if (material_name == m_attributes.materials[i])
            return i;
    }
    throw any_error("logic error");
}

// Returns \c true if group is active in group_bitfield.
bool belongs_in_groups(uint32 group_bitfield, uint32 group)
{
    return ((group_bitfield >> group) & 0x1) == 1;
}

// Returns \c true if both groups shares any single group
bool shares_a_group(uint32 group_bits0, uint32 group_bits1)
{
    return (group_bits0 & group_bits1) != 0;
}

/// Calculates polygons corner normal, by averaging polygon normals in same
/// smoothing groups.
void topology::calculate_corner_normal(polygon* polygon, corner_access corner,
    polygon_attribute<uint32> const& smoothing_groups)
{
    uint32 groups_bitfield = smoothing_groups.empty()
        ? 01U : smoothing_groups.value_for(polygon);

    //vector3f normal = m_attributes.face_normal.value_for(polygon);
    //vector3f tangent = m_attributes.polygon_tangent.value_for(polygon);
    //vector3f bitangent = m_attributes.polygon_bitangent.value_for(polygon);

    polygon_data const& p_data
        = m_attributes.polygon.value_for(polygon);

    vector3f normal = p_data.normal;
    vector3f tangent = p_data.tangent;
    vector3f bitangent = p_data.bitangent;

    // Iterate for every polygon connected to the corner's point.
    point const* point = corner.get().point();

    for (size_t i = 0, i_end = point->corner_count(); i < i_end; ++i)
    {
        p3d_format::polygon const* other = point->corner_at(i).polygon();
        if (other == polygon)
            continue;

        polygon_data const& other_data
            = m_attributes.polygon.value_for(other);

        uint32 other_bitfield = smoothing_groups.empty()
            ? 01U : smoothing_groups.value_for(other);

        if (shares_a_group(groups_bitfield, other_bitfield))
        {
            // Both polygons belong in same smoothing group.
            //normal += m_attributes.face_normal.value_for(other);
            //tangent += m_attributes.polygon_tangent.value_for(other);
            //bitangent += m_attributes.polygon_bitangent.value_for(other);

            // HACK with combined positions.
            // Some triangles are discarded, so neighbor triangles might not
            // be correct.
            if (dot(normal, other_data.normal) > -0.5)
            {
                normal += other_data.normal;
                tangent += other_data.tangent;
                bitangent += other_data.bitangent;
            }
        }
    }

    normal = normalize(normal);
    //tangent = normalize(tangent);
    //bitangent = normalize(bitangent);

    corner_data data;

    data.normal = normal; // normalize(normal);

    // Gram-Schmidt orthogonalize
    tangent = normalize(tangent - normal * dot(normal, tangent));
        
    // Calculate handedness
    float handedness = (dot(cross(normal, tangent), bitangent) < 0.0f)
        ? -1.0f : 1.0f;
    vector4f corner_tangent(tangent, handedness);

    data.tangent = corner_tangent;

    m_attributes.corner.add(corner, static_cast<corner_data&&>(data)); // move
}

// Free functions

namespace {
    template<typename T>
    bool within_epsilon(corner_attribute<T> const& attributes,
        corner_access const& corner_0, corner_access const& corner_1)
    {
        corner_data const& data_0 = attributes.value_for(corner_0);
        corner_data const& data_1 = attributes.value_for(corner_1);

        if (!within_epsilon(data_0.normal, data_1.normal)
            || !within_epsilon(data_0.tangent, data_1.tangent))
        {
            return false;
        }

        //assert(data_0.texcoords.size() == data_1.texcoords.size());

        //for (size_t i = 0; i < data_0.texcoords.size(); ++i)
        //{
        //    if (!within_epsilon(data_0.texcoords[i], data_1.texcoords[i]))
        //        return false;
        //}

        return true;
    }

    bool within_epsilon(corner_attribute<vector3f> const& attributes,
        corner_access const& corner_0, corner_access const& corner_1)
    {
        vector3f const& data_0 = attributes.value_for(corner_0);
        vector3f const& data_1 = attributes.value_for(corner_1);

        return within_epsilon(data_0, data_1);
    }
} // anonymous

bool are_corners_equal(corner_access const& corner_0,
    corner_access const& corner_1, topology const& topology)
{
    if (!within_epsilon(topology.attributes().corner, corner_0, corner_1))
        return false;

    for (std::vector<corner_attribute<vector3f> >::const_iterator i
        = topology.attributes().texcoords.begin();
        i != topology.attributes().texcoords.end(); ++i)
    {
        if (!within_epsilon(*i, corner_0, corner_1))
            return false;
    }

    return true;
}

} // namespace p3d_format
