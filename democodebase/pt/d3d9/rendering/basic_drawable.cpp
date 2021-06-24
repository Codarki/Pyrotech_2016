#include "basic_drawable.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/index_buffer.h"
#include "pt/d3d9/vertex_streams.h"

namespace pt { namespace d3d9 { namespace rendering {

basic_drawable::basic_drawable(
	std::shared_ptr<vertex_streams> streams,
    std::unique_ptr<index_buffer>&& indices)
: m_streams(streams)
, m_indices(move(indices))
{
    assert(m_streams);
}

basic_drawable::~basic_drawable()
{
}

void basic_drawable::draw() const
{
    m_streams->use();
    m_indices->use();

    INT base_vertex_index = 0;
    UINT min_index = 0;
    UINT num_vertices = static_cast<UINT>(m_streams->vertex_count());
    UINT start_index = 0;
    UINT primitive_count = static_cast<UINT>(m_indices->count() / 3);

    graphics_device& device = m_indices->mutable_device();

    set_fvf(device, 0);

    d3d9::draw_indexed_primitive(device, D3DPT_TRIANGLELIST,
        base_vertex_index, min_index, num_vertices, start_index,
        primitive_count);
}

}}} // namespace pt::d3d9::rendering
