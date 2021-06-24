#ifndef CONTENT_BOX_VERTEX_STREAM_H
#define CONTENT_BOX_VERTEX_STREAM_H

#include "pt/d3d9/stream_source.h"
#include "pt/d3d9/vertex_declaration.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {
    class vertex_buffer;
}}

namespace content {

using namespace pt;

class box_vertex_stream : public d3d9::stream_source
{
public:
    box_vertex_stream(d3d9::graphics_device& device, float side_length);

    std::vector<size_t> const& indices() const;

public: // interface required by d3d9::rendering::stream_source
    void use(int stream_number) const;
    d3d9::vertex_stream_declaration const& declaration() const;
    size_t vertex_count() const;

private: // data members
    size_t m_vertex_count;
    std::unique_ptr<d3d9::vertex_buffer> m_buffer;
    d3d9::vertex_stream_declaration m_declaration;
    std::vector<size_t> m_indices;
};

} // namespace content

#endif
