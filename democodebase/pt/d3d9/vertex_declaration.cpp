#include "vertex_declaration.h"
#include "device_convenience.h"
#include "enum_utility.h"
#include "pt/any_error.h"
#include "pt/none.h"
#include "pt/input_stream.h"
#include "pt/output_stream.h"

namespace pt { namespace d3d9 {

namespace {
} // anonymous namespace

// vertex_stream_declaration

vertex_stream_declaration::vertex_stream_declaration()
:   m_stream(none<size_t>())
,   m_last_offset(0)
{
}

vertex_stream_declaration::vertex_stream_declaration(size_t stream)
:   m_stream(stream)
,   m_last_offset(0)
{
}

void vertex_stream_declaration::push_back(D3DDECLTYPE type,
    D3DDECLUSAGE usage, BYTE index)
{
    D3DVERTEXELEMENT9 element;
    element.Stream = static_cast<WORD>(m_stream);
    element.Offset = static_cast<WORD>(m_last_offset);
    element.Type = static_cast<BYTE>(type);
    element.Method = static_cast<BYTE>(D3DDECLMETHOD_DEFAULT);
    element.Usage = static_cast<BYTE>(usage);
    element.UsageIndex = index;
    
    m_elements.push_back(element);

    m_last_offset += declaration_size_in_bytes(type);
}

void vertex_stream_declaration::push_back(D3DVERTEXELEMENT9 const& element)
{
    //assert(m_stream == element.Stream);

    m_elements.push_back(element);
    m_last_offset += declaration_size_in_bytes(
        static_cast<D3DDECLTYPE>(element.Type));
}

size_t vertex_stream_declaration::stride() const
{
    return m_last_offset;
}

std::vector<D3DVERTEXELEMENT9> const&
    vertex_stream_declaration::elements() const
{
    return m_elements;
}

// vertex_declaration

vertex_declaration::vertex_declaration(graphics_device& device)
:   device_child(device)
{
}

size_t vertex_declaration::stream_count() const
{
    return m_streams.size();
}

vertex_stream_declaration const& vertex_declaration::stream(
    size_t index) const
{
    assert(index < stream_count());
    return m_streams[index];
}

vertex_stream_declaration& vertex_declaration::stream(size_t index)
{
    if (index >= m_streams.size())
        m_streams.resize(index + 1);

    return m_streams[index];
}

void vertex_declaration::finalize()
{
    std::vector<D3DVERTEXELEMENT9> elements;

    for (size_t i = 0; i < m_streams.size(); ++i)
    {
        for (size_t j = 0; j < m_streams[i].elements().size(); ++j)
        {
            D3DVERTEXELEMENT9 element = m_streams[i].elements()[j];
            element.Stream = static_cast<WORD>(i);

            elements.push_back(element);
        }
    }
    D3DVERTEXELEMENT9 end = D3DDECL_END();
    elements.push_back(end);

    m_native = create_vertex_declaration(device(), &elements[0]);
}

bool vertex_declaration::valid() const
{
    return m_native != 0;
}

void vertex_declaration::use() const
{
    set_vertex_declaration(mutable_device(), m_native);
}

void vertex_declaration::before_destruction()
{
    //assert(!"todo");
}

void vertex_declaration::before_reset()
{
    m_native.reset();
}

void vertex_declaration::after_reset()
{
    finalize();
}

//

void write_vertex_declaration_dump(
    vertex_declaration const& declaration, output_stream& output)
{
    pt::write(output, uint32(declaration.stream_count()));

    for (size_t i = 0; i < declaration.stream_count(); ++i)
    {
        write_vertex_stream_declaration_dump(declaration.stream(i), output);
    }
}

vertex_declaration read_vertex_declaration_dump(graphics_device& device,
    input_stream& input)
{
    vertex_declaration result(device);

    size_t stream_count = pt::read<uint32>(input);

    for (size_t i = 0; i < stream_count; ++i)
    {
        vertex_stream_declaration stream
            = read_vertex_stream_declaration_dump(i, input);

        result.stream(i) = stream;
    }
    return result;
}

void write_vertex_stream_declaration_dump(
    vertex_stream_declaration const& declaration, output_stream& output)
{
    std::vector<D3DVERTEXELEMENT9> const& elements = declaration.elements();

    pt::write(output, uint32(elements.size()));

    for (size_t i = 0; i < elements.size(); ++i)
    {
        pt::write(output, uint16(elements[i].Stream));
        pt::write(output, uint16(elements[i].Offset));
        pt::write(output, uint8(elements[i].Type));
        pt::write(output, uint8(elements[i].Method));
        pt::write(output, uint8(elements[i].Usage));
        pt::write(output, uint8(elements[i].UsageIndex));
    }
}

vertex_stream_declaration read_vertex_stream_declaration_dump(
    size_t stream_index, input_stream& input)
{
    vertex_stream_declaration stream(stream_index);

    size_t element_count = pt::read<uint32>(input);
    for (size_t i = 0; i < element_count; ++i)
    {
        D3DVERTEXELEMENT9 element;
        element.Stream = pt::read<uint16>(input);
        element.Offset = pt::read<uint16>(input);
        element.Type = pt::read<uint8>(input);
        element.Method = pt::read<uint8>(input);
        element.Usage = pt::read<uint8>(input);
        element.UsageIndex = pt::read<uint8>(input);

        stream.push_back(element);
    }

    return stream;
}

}} // namespace pt::d3d9
