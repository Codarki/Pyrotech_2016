#include "box_vertex_stream.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/vertex_buffer.h"

namespace content {

box_vertex_stream::box_vertex_stream(d3d9::graphics_device& device,
    float side_length)
{
    m_declaration.push_back(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0);

    std::vector<vector3f> vertices;
    vertices.push_back(vector3f(-1, -1, 1));
    vertices.push_back(vector3f( 1, -1, 1));
    vertices.push_back(vector3f( 1,  1, 1));
    vertices.push_back(vector3f(-1,  1, 1));
    vertices.push_back(vector3f(-1, -1, -1));
    vertices.push_back(vector3f( 1, -1, -1));
    vertices.push_back(vector3f( 1,  1, -1));
    vertices.push_back(vector3f(-1,  1, -1));

    std::vector<vector3i> indices3;
    // front.
    indices3.push_back(vector3i(0, 1, 2));
    indices3.push_back(vector3i(0, 2, 3));
    // back.
    indices3.push_back(vector3i(4, 6, 5));
    indices3.push_back(vector3i(4, 7, 6));
    // left.
    indices3.push_back(vector3i(4, 0, 3));
    indices3.push_back(vector3i(4, 3, 7));
    // right.
    indices3.push_back(vector3i(1, 5, 6));
    indices3.push_back(vector3i(1, 6, 2));
    // top.
    indices3.push_back(vector3i(4, 5, 1));
    indices3.push_back(vector3i(4, 1, 0));
    // bottom.
    indices3.push_back(vector3i(3, 2, 6));
    indices3.push_back(vector3i(3, 6, 7));

    for (std::vector<vector3i>::const_iterator i = indices3.begin();
        i != indices3.end(); ++i)
    {
        vector3i const& j = *i;
        m_indices.push_back(j[0]);
        m_indices.push_back(j[1]);
        m_indices.push_back(j[2]);
    }

    m_vertex_count = vertices.size();

    DWORD bytes = static_cast<DWORD>(m_vertex_count * m_declaration.stride());

    m_buffer.reset(new d3d9::vertex_buffer(device, bytes, D3DUSAGE_WRITEONLY,
        0, D3DPOOL_DEFAULT));

    d3d9::vertex_buffer_lock lock(m_buffer->native(), 0, bytes, 0);

    float* data = lock.data<float>();

    // For each vertex, write all elements.
    for (size_t i = 0; i < m_vertex_count; ++i)
    {
        *data++ = vertices[i][0] * 0.5f * side_length;
        *data++ = vertices[i][1] * 0.5f * side_length;
        *data++ = vertices[i][2] * 0.5f * side_length;
    }
}

std::vector<size_t> const& box_vertex_stream::indices() const
{
    return m_indices;
}

void box_vertex_stream::use(int stream_number) const
{
    UINT offset_in_bytes = 0;
    UINT stride = static_cast<UINT>(m_declaration.stride());

    set_stream_source(m_buffer->device(), static_cast<UINT>(stream_number),
        m_buffer->native(), offset_in_bytes, stride);
}

d3d9::vertex_stream_declaration const& box_vertex_stream::declaration() const
{
    return m_declaration;
}

size_t box_vertex_stream::vertex_count() const
{
    return m_vertex_count;
}

} // namespace content
