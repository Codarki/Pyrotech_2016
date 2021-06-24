#include "cylinder.h"
#include "deformer.h"
#include "pt/d3d9/rendering/basic_drawable.h"
#include "pt/d3d9/rendering/static_vertex_stream.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/vertex_streams.h"
#include "pt/boost/cast.h"

#include "pt/d3d9/rendering/attribute_collection.h"
#include "pt/d3d9/rendering/source/vertex_attribute.h"
#include "pt/d3d9/rendering/item.h"
#include "pt/d3d9/rendering/transformer.h"
#include "pt/d3d9/rendering/transformation.h"
#include "pt/matrix_operations.h"
#include "pt/std/algorithm.h"

namespace pt { namespace d3d9 { namespace rendering {
    class surface;
}}}

namespace content { namespace sequences { namespace detail {

namespace {
    std::unique_ptr<d3d9::index_buffer> do_create(d3d9::graphics_device& device,
        std::vector<vector3i> const& indices)
    {
        size_t count = indices.size() * 3;
        UINT bytes = static_cast<UINT>(count * sizeof(uint16));
        std::unique_ptr<d3d9::index_buffer> buffer
            = d3d9::create_default_16bit_unique_index_buffer(device, bytes);

        d3d9::scoped_index_buffer_lock lock(*buffer);

        uint16* dst = lock.data();

        for (size_t i = 0; i < indices.size(); ++i)
        {
            *dst++ = numeric_cast<uint16>(indices[i][0]);
            *dst++ = numeric_cast<uint16>(indices[i][1]);
            *dst++ = numeric_cast<uint16>(indices[i][2]);
        }

        return buffer;
    }
}

cylinder::cylinder(d3d9::graphics_device& device,
	std::shared_ptr<d3d9::vertex_streams> const& streams,
    std::vector<vector3i> const& indices)
{
    streams->finalize();

    m_drawable.reset(new d3d9::rendering::basic_drawable(
        streams, do_create(device, indices)));
}

box3f const& cylinder::bounding_box() const
{
    return m_bounding_box;
}
d3d9::rendering::drawable& cylinder::drawable()
{
    return *m_drawable;
}
d3d9::rendering::drawable const& cylinder::drawable() const
{
    return *m_drawable;
}

d3d9::rendering::surface const& cylinder::surface() const
{
    assert(false && "hahahaa lets crash");
    void* crash = 0;
    return *reinterpret_cast<d3d9::rendering::surface*>(crash);
}

d3d9::rendering::surface& cylinder::surface()
{
    assert(false && "hahahaa lets crash");
    void* crash = 0;
    return *reinterpret_cast<d3d9::rendering::surface*>(crash);
}

// line_list_drawable

line_list_drawable::line_list_drawable(d3d9::graphics_device& device,
    d3d9::vertex_streams const* streams)
:   d3d9::device_reference(device)
,   m_streams(streams)
{
    assert(m_streams);
}

void line_list_drawable::draw() const
{
    m_streams->use();

    INT base_vertex_index = 0;
    UINT num_vertices = static_cast<UINT>(m_streams->vertex_count());
    UINT primitive_count = num_vertices / 2;

    d3d9::graphics_device& device = mutable_device();

    d3d9::set_fvf(device, 0);

    //scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_CCW);
    //scoped_render_state zenable(device, D3DRS_ZENABLE, TRUE);
    ////scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);

    d3d9::draw_primitive(device, D3DPT_LINELIST,
        base_vertex_index, primitive_count);
}

d3d9::rendering::surface const& line_list_drawable::surface() const
{
    assert(false && "can't return anything, lets crash");
    void* crash = 0;
    return *reinterpret_cast<d3d9::rendering::surface const*>(crash);
}

d3d9::rendering::surface& line_list_drawable::surface()
{
    assert(false && "can't return anything, lets crash");
    void* crash = 0;
    return *reinterpret_cast<d3d9::rendering::surface*>(crash);
}

// line_list

line_list::line_list(d3d9::graphics_device& device,
    std::shared_ptr<d3d9::vertex_streams> const& streams)
:   m_streams(streams)
{
    m_streams->finalize();

    m_drawable.reset(new line_list_drawable(device, m_streams.get()));
}

box3f const& line_list::bounding_box() const
{
    return m_bounding_box;
}

d3d9::rendering::drawable& line_list::drawable()
{
    return *m_drawable;
}

d3d9::rendering::drawable const& line_list::drawable() const
{
    return *m_drawable;
}

d3d9::rendering::surface const& line_list::surface() const
{
    assert(false && "can't return anything, lets crash");
    void* crash = 0;
    return *reinterpret_cast<d3d9::rendering::surface*>(crash);
}

d3d9::rendering::surface& line_list::surface()
{
    assert(false && "can't return anything, lets crash");
    void* crash = 0;
    return *reinterpret_cast<d3d9::rendering::surface*>(crash);
}

// cylinder_source

namespace {
    struct deformer_distance
    {
        float distance;
        size_t index;
    };
    bool by_distance(deformer_distance const& a, deformer_distance const& b)
    {
        return a.distance < b.distance;
    }
}

cylinder_source::cylinder_source(float height, float radius,
    deformer_collection const& deformers,
    std::shared_ptr<d3d9::rendering::item> parent)
{
    float pi_2 = 2.0f * 3.1415f;
    float y_offset = 0.0f; // - height / 2.0f

    // Create coordinates.
    for (size_t y = 0; y < height_segments + 1; ++y)
    {
        vector3f position;
        vector2f texcoord;

        // y and v coordinates.
        texcoord[1] = static_cast<float>(y) / height_segments;
        position[1] = texcoord[1] * height + y_offset;

        for (int i = 0; i < radius_segments; ++i)
        {
            float r = i * pi_2 / radius_segments;
            // circle in xz-plane.

            position[0] = std::cos(r) * radius;
            position[2] = std::sin(r) * radius;
            texcoord[0] = r / pi_2;

            m_positions.push_back(position);
            m_texcoords.push_back(texcoord);
        }
    }

    // Create indices.
    for (int y0 = 0; y0 < height_segments; ++y0)
    {
        int y1 = y0 + 1;
        assert(y1 < height_segments + 1);

        for (int i0 = 0; i0 < radius_segments; ++i0)
        {
            int i1 = (i0 + 1) < radius_segments ? i0 + 1 : 0;

            // Create quad.
            vector3i triangle_0;
            triangle_0[0] = offset(y0, i0);
            triangle_0[1] = offset(y0, i1);
            triangle_0[2] = offset(y1, i0);

            vector3i triangle_1;
            triangle_1[0] = offset(y1, i0);
            triangle_1[1] = offset(y1, i1);
            triangle_1[2] = offset(y0, i1);

            m_indices.push_back(triangle_0);
            m_indices.push_back(triangle_1);
        }
    }

    // Create deformer indices and weights
    m_skin_indices.resize(m_positions.size());
    m_skin_weights.resize(m_positions.size());

    for (size_t i = 0; i < m_positions.size(); ++i)
    {
        std::vector<deformer_distance> distances(deformers.size());

        for (size_t j = 0; j < deformers.size(); ++j)
        {
            float time = 0.0f;
            matrix4x4f deformer_to_root = local_to_parent(
                deformers.at(j).item()->transformer().transformation(),
                &parent->transformer().transformation(), time);

            vector3f tip_at_root = transform_position(
                vector3f(0, 0, deformers.at(j).length()),
                deformer_to_root);

            float dist = distance(m_positions[i], tip_at_root);

            distances[j].index = j;
            distances[j].distance = dist;
        }

        std::sort(distances.begin(), distances.end(), by_distance);

        float weight_radius = 4.0f;
        vector2f weights;
        weights[0] = (weight_radius - distances[0].distance) / weight_radius;
        weights[1] = (weight_radius - distances[1].distance) / weight_radius;
        weights[0] = clamp(weights[0], 0.0f, 1.0f);
        weights[1] = clamp(weights[1], 0.0f, 1.0f);

        weights = normalize(weights);

        m_skin_indices[i][0] = numeric_cast<uint8>(distances[0].index);
        m_skin_indices[i][1] = numeric_cast<uint8>(distances[1].index);

        m_skin_weights[i][0] = weights[0];
        m_skin_weights[i][1] = weights[1];
    }
}

std::vector<vector3f> const& cylinder_source::positions() const
{
    return m_positions;
}
std::vector<vector2f> const& cylinder_source::texcoords() const
{
    return m_texcoords;
}
std::vector<vector3i> const& cylinder_source::indices() const
{
    return m_indices;
}
std::vector<vector<uint8,4> > const& cylinder_source::skin_indices() const
{
    return m_skin_indices;
}
std::vector<vector4f> const& cylinder_source::skin_weights() const
{
    return m_skin_weights;
}
int cylinder_source::offset(int y, int i)
{
    return y * radius_segments + i;
}

// free functions

std::shared_ptr<line_list> build_bone(d3d9::graphics_device& device, float length,
    vector3f color)
{
    // Create source data.
    std::vector<vector3f> source_positions;
    std::vector<vector4f> source_colors;

    float half_width = length / 8.0f;
    float middle_length = length / 3.0f;

    //vector3f tip(0, 0, length);
    //vector3f root(0, 0, 0);
    //vector3f middle[4];
    //middle[0] = vector3f(-half_width, -half_width, middle_length);
    //middle[1] = vector3f(half_width, -half_width, middle_length);
    //middle[2] = vector3f(half_width, half_width, middle_length);
    //middle[3] = vector3f(-half_width, half_width, middle_length);

    vector3f tip(0, 0, length);
    vector3f root(0, 0, 0);
    vector3f middle[4];
    middle[0] = vector3f(-half_width, -half_width, middle_length);
    middle[1] = vector3f(half_width, -half_width, middle_length);
    middle[2] = vector3f(half_width, half_width, middle_length);
    middle[3] = vector3f(-half_width, half_width, middle_length);

    source_positions.push_back(root);
    source_positions.push_back(middle[0]);
    source_positions.push_back(root);
    source_positions.push_back(middle[1]);
    source_positions.push_back(root);
    source_positions.push_back(middle[2]);
    source_positions.push_back(root);
    source_positions.push_back(middle[3]);

    source_positions.push_back(middle[0]);
    source_positions.push_back(middle[1]);
    source_positions.push_back(middle[1]);
    source_positions.push_back(middle[2]);
    source_positions.push_back(middle[2]);
    source_positions.push_back(middle[3]);
    source_positions.push_back(middle[3]);
    source_positions.push_back(middle[0]);

    source_positions.push_back(tip);
    source_positions.push_back(middle[0]);
    source_positions.push_back(tip);
    source_positions.push_back(middle[1]);
    source_positions.push_back(tip);
    source_positions.push_back(middle[2]);
    source_positions.push_back(tip);
    source_positions.push_back(middle[3]);

    for (std::vector<vector3f>::const_iterator i = source_positions.begin();
        i != source_positions.end(); ++i)
    {
        source_colors.push_back(color);
    }

    using namespace pt::d3d9;
    using namespace pt::d3d9::rendering;

    // Create source vertex attribute maps.
    std::shared_ptr<source::vectorf_attribute<3> > positions(
        new source::vectorf_attribute<3>(source_positions));
    std::shared_ptr<source::vectorf_attribute<4> > colors(
        new source::vectorf_attribute<4>(source_colors));

    attribute_collection vertex_attributes;
    vertex_attributes.add(semantic("POSITION"), positions);
    vertex_attributes.add(semantic("COLOR"), colors);

    // Create vertex buffer.
    std::shared_ptr<vertex_streams> streams(new vertex_streams(device));
    streams->add(std::make_shared<static_vertex_stream>(device, vertex_attributes));

    // Create geometry.
    std::shared_ptr<line_list> geometry(new line_list(device, streams));

    return geometry;
}

std::shared_ptr<cylinder> build_cylinder(d3d9::graphics_device& device,
    deformer_collection const& deformers,
    std::shared_ptr<d3d9::rendering::item> parent)
{
    cylinder_source source(10.0f, 2.0f, deformers, parent);

    using namespace pt::d3d9;
    using namespace pt::d3d9::rendering;

    // Create source vertex attribute maps.
    std::shared_ptr<source::vectorf_attribute<3> > positions(
        new source::vectorf_attribute<3>(source.positions()));
    std::shared_ptr<source::vectorf_attribute<2> > texcoords(
        new source::vectorf_attribute<2>(source.texcoords()));

    std::shared_ptr<source::ubyte4_attribute> skin_indices(
        new source::ubyte4_attribute(source.skin_indices()));
    std::shared_ptr<source::vectorf_attribute<4> > skin_weights(
        new source::vectorf_attribute<4>(source.skin_weights()));

    attribute_collection vertex_attributes;
    vertex_attributes.add(semantic("POSITION"), positions);
    vertex_attributes.add(semantic("TEXCOORD"), texcoords);
    vertex_attributes.add(semantic("BLENDWEIGHT"), skin_weights);
    vertex_attributes.add(semantic("BLENDINDICES"), skin_indices);

    // Create vertex buffer.
    std::shared_ptr<vertex_streams> streams(new vertex_streams(device));
    streams->add(std::make_shared<static_vertex_stream>(device, vertex_attributes));

    // Create geometry.
    std::shared_ptr<cylinder> geometry(
        new cylinder(device, streams, source.indices()));

    return geometry;
}

}}} // namespace content::sequences::detail
