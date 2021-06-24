#ifndef PT_D3D9_VERTEX_DECLARATION_H
#define PT_D3D9_VERTEX_DECLARATION_H

#include "device_child.h"
#include "native.h"
#include "pt/com_ptr.h"
#include "pt/std/vector.h"

namespace pt {
    class input_stream;
    class output_stream;
}

namespace pt { namespace d3d9 {

/// Declares one vertex stream.
class vertex_stream_declaration
{
public:
    vertex_stream_declaration();
    vertex_stream_declaration(size_t stream);

    void push_back(D3DDECLTYPE type, D3DDECLUSAGE usage, BYTE index);
    void push_back(D3DVERTEXELEMENT9 const& element);
    size_t stride() const;
    std::vector<D3DVERTEXELEMENT9> const& elements() const;

private: // data members
    size_t m_stream;
    size_t m_last_offset;
    std::vector<D3DVERTEXELEMENT9> m_elements;
};

/// Direct3D9 vertex declaration. A vertex declaration is an object that
/// defines the data members of a vertex (i.e. texture coordinates, colors,
/// normals, etc.). 
class vertex_declaration : public device_child
{
public:
    vertex_declaration(graphics_device& device);

    size_t stream_count() const;
    vertex_stream_declaration const& stream(size_t index) const;
    vertex_stream_declaration& stream(size_t index);
    void finalize();
    bool valid() const;

    void use() const;

protected: // interface required by device_child
    void before_destruction();
    void before_reset();
    void after_reset();

private: // data members
    std::vector<vertex_stream_declaration> m_streams;
    com_ptr<IDirect3DVertexDeclaration9> m_native;
};

void write_vertex_declaration_dump(vertex_declaration const& declaration,
    output_stream& output);
vertex_declaration read_vertex_declaration_dump(graphics_device& device,
    input_stream& input);

void write_vertex_stream_declaration_dump(
    vertex_stream_declaration const& declaration, output_stream& output);
vertex_stream_declaration read_vertex_stream_declaration_dump(
    size_t stream_index, input_stream& input);

}} // namespace pt::d3d9

#endif
