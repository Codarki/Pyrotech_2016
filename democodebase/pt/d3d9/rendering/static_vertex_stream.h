#ifndef PT_D3D9_RENDERING_STATIC_VERTEX_STREAM_H
#define PT_D3D9_RENDERING_STATIC_VERTEX_STREAM_H

#include "pt/d3d9/stream_source.h"
#include "pt/d3d9/vertex_buffer.h"
#include "pt/d3d9/vertex_declaration.h"
#include "pt/std/memory.h"

namespace pt {
    class input_stream;
    class output_stream;
}

namespace pt { namespace d3d9 { namespace rendering {

class attribute_collection;

/// Static vertex stream.
class static_vertex_stream : public stream_source
{
public:
    static_vertex_stream(graphics_device& device,
        attribute_collection const& vertex_attributes);

    static_vertex_stream(vertex_stream_declaration const& declaration,
		std::shared_ptr<vertex_buffer> buffer, size_t vertex_count);

public: // interface required by stream_source
    void use(int stream_number) const;
    vertex_stream_declaration const& declaration() const;
    size_t vertex_count() const;

private: // data members
    size_t m_vertex_count;
    std::shared_ptr<vertex_buffer> m_buffer;
    vertex_stream_declaration m_declaration;
};

void write_static_vertex_stream_dump(
    attribute_collection const& vertex_attributes, output_stream& output);
std::shared_ptr<static_vertex_stream> read_static_vertex_stream_dump(
    graphics_device& device, input_stream& input);

}}} // namespace pt::d3d9::rendering

#endif
