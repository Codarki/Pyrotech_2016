#ifndef CONTENT_SEQUENCES_DETAIL_CYLINDER_H
#define CONTENT_SEQUENCES_DETAIL_CYLINDER_H

#include "pt/d3d9/rendering/geometry.h"
#include "pt/d3d9/rendering/drawable.h"
#include "pt/d3d9/index_buffer.h"
#include "pt/box.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {
    class graphics_device;
    class vertex_streams;

    namespace rendering {
        class basic_drawable;
        class item;
    }
}}

namespace content { namespace sequences { namespace detail {

using namespace pt;

class cylinder : public d3d9::rendering::geometry
{
public:
    cylinder(d3d9::graphics_device& device,
		std::shared_ptr<d3d9::vertex_streams> const& streams,
        std::vector<vector3i> const& indices);

public: // interface required by d3d9::rendering::geometry
    box3f const& bounding_box() const;
    d3d9::rendering::drawable& drawable();
    d3d9::rendering::drawable const& drawable() const;
    d3d9::rendering::surface const& surface() const;
    d3d9::rendering::surface& surface();

private: // data members
    box3f m_bounding_box;
    std::shared_ptr<d3d9::rendering::basic_drawable> m_drawable;
};

class line_list_drawable : public d3d9::rendering::drawable,
    d3d9::device_reference
{
public:
    line_list_drawable(d3d9::graphics_device& device,
        d3d9::vertex_streams const* streams);

public: // interface required by d3d9::rendering::drawable
    void draw() const;
    d3d9::rendering::surface const& surface() const;
    d3d9::rendering::surface& surface();

private: // data members
    d3d9::vertex_streams const* m_streams;
};

class line_list : public d3d9::rendering::geometry
{
public:
    line_list(d3d9::graphics_device& device,
        std::shared_ptr<d3d9::vertex_streams> const& streams);

public: // interface required by d3d9::rendering::geometry
    box3f const& bounding_box() const;
    d3d9::rendering::drawable& drawable();
    d3d9::rendering::drawable const& drawable() const;
    d3d9::rendering::surface const& surface() const;
    d3d9::rendering::surface& surface();

private: // data members
    box3f m_bounding_box;
    std::shared_ptr<d3d9::vertex_streams> m_streams;
    std::shared_ptr<d3d9::rendering::drawable> m_drawable;
};

class deformer_collection;

class cylinder_source
{
public:
    static int const height_segments = 16;
    static int const radius_segments = 16;

public:
    cylinder_source(float height, float radius,
        deformer_collection const& deformers,
        std::shared_ptr<d3d9::rendering::item> parent);

    std::vector<vector3f> const& positions() const;
    std::vector<vector2f> const& texcoords() const;
    std::vector<vector3i> const& indices() const;
    std::vector<vector<uint8,4> > const& skin_indices() const;
    std::vector<vector4f> const& skin_weights() const;

private: // helper functions
    int offset(int y, int i);

private: // data members
    std::vector<vector3f> m_positions;
    std::vector<vector2f> m_texcoords;
    std::vector<vector3i> m_indices;
    std::vector<vector<uint8,4> > m_skin_indices;
    std::vector<vector4f> m_skin_weights;
};

std::shared_ptr<line_list> build_bone(d3d9::graphics_device& device, float length,
    vector3f color);
std::shared_ptr<cylinder> build_cylinder(d3d9::graphics_device& device,
    deformer_collection const& deformers,
    std::shared_ptr<d3d9::rendering::item> parent);

}}} // namespace content::sequences::detail

#endif
