#ifndef PT_P3D_FORMAT_TOPOLOGY_H
#define PT_P3D_FORMAT_TOPOLOGY_H

#include "point_attribute.h"
#include "point.h"
#include "polygon.h"
#include "pt/basic_types.h"
#include "pt/vector.h"
#include "pt/format.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/vector.h"
#include "pt/exceptions/contextual_exception.h"

namespace pt {
class progress_report;
}

namespace p3d_format {

using namespace pt;

class mesh;
class topology;

struct corner_data
{
    //std::vector<vector3f> texcoords;

    // The handedness of the local coordinate system is stored in the
    // w-coordinate.
    vector4f tangent;
    vector3f normal;
};

struct polygon_data
{
    vector3f normal;
    uint32 material_indices;
    vector3f tangent;
    vector3f bitangent;
};

struct attributes
{
    point_attribute<vector3f> position;
    polygon_attribute<polygon_data> polygon;

    //polygon_attribute<vector3f> face_normal;
    //polygon_attribute<uint32> material_indices;
    std::vector<corner_attribute<vector3f> > texcoords;
    std::vector<std::string> materials;

    //polygon_attribute<vector3f> polygon_tangent;
    //polygon_attribute<vector3f> polygon_bitangent;

    corner_attribute<corner_data> corner;
};

class topology_building_error : public pt::exceptions::contextual_exception
{
public:
    topology_building_error(std::string const& reason)
    {
        this->set_text(pt::format("Building topology failed: %1%") % reason);
    }
};

struct topology_population_result
{
    topology_population_result()
    : collapsed_triangle_count(0)
    , ignored_triangles_because_surface_area_is_below_epsilon(0)
    , triangle_count(0)
    , failed_tangent_calculation_count(0)
    {
    }
    size_t collapsed_triangle_count;
    size_t ignored_triangles_because_surface_area_is_below_epsilon;
    size_t triangle_count;
    size_t failed_tangent_calculation_count;
};

class topology
{
public:
    //topology(mesh const& mesh);

    topology_population_result populate(mesh const& mesh, progress_report* progress);

    std::vector<corner_access> const& corners() const;
    std::vector<point> const& points() const;
    std::vector<polygon> const& polygons() const;

    p3d_format::attributes const& attributes() const;

private: // helper functions
    size_t material_index(std::string const& material_name) const;
    void calculate_corner_normal(polygon* polygon, corner_access corner,
        polygon_attribute<uint32> const& smoothing_groups);

private: // data members
    std::vector<corner_access> m_corners;
    //std::vector<shared_ptr<point> > m_points;
    //std::vector<shared_ptr<polygon> > m_polygons;

    std::vector<point> m_points;
    std::vector<polygon> m_polygons;

    p3d_format::attributes m_attributes;
};

bool are_corners_equal(corner_access const& corner_0,
    corner_access const& corner_1, topology const& topology);

} // namespace p3d_format

#include "attribute_map.inl"

#endif
