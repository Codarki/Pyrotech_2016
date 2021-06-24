#include "static_vertex_stream.h"
#include "attribute_collection.h"
#include "source/vertex_attribute.h"
#include "pt/d3d9/vertex_streams.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/input_stream.h"
#include "pt/output_stream.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
	std::vector<std::shared_ptr<source::vertex_attribute const> >
        create_attributes(attribute_collection const& vertex_attributes)
    {
        // Build vertex declaration from semantics, and corresponding attribute
        // vector.

        // Build explicit attributes first. ie POSITION0
		std::shared_ptr<source::vertex_attribute const> position_source
            = vertex_attributes.attribute(semantic("POSITION"));

		std::vector<std::shared_ptr<source::vertex_attribute const> > attributes;
        attributes.push_back(position_source);

        // Then implicit attributes.
        for (attribute_collection::const_iterator i = vertex_attributes.begin();
            i != vertex_attributes.end(); ++i)
        {
            semantic const& semantic = i->first;
            std::shared_ptr<source::vertex_attribute> attribute = i->second;

            // Skip explicit attributes
            if (semantic.text() == "POSITION0")
                continue;

            attributes.push_back(attribute);
        }
        return attributes;
    }

    vertex_stream_declaration create_vertex_stream_declaration(
        attribute_collection const& vertex_attributes)
    {
        vertex_stream_declaration declaration;

        // Build explicit attributes first. ie POSITION0
        declaration.push_back(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0);

        // Then implicit attributes.
        for (attribute_collection::const_iterator i = vertex_attributes.begin();
            i != vertex_attributes.end(); ++i)
        {
            semantic const& semantic = i->first;
            std::shared_ptr<source::vertex_attribute> attribute = i->second;

            // Skip explicit attributes
            if (semantic.text() == "POSITION0")
                continue;

            D3DDECLTYPE decl_type = declaration_type(
                attribute->element_type(), attribute->element_count());
            D3DDECLUSAGE usage = d3d9_usage(semantic.name());
            BYTE index = static_cast<BYTE>(semantic.index());
            declaration.push_back(decl_type, usage, index);
        }
        return declaration;
    }
} // anonymous namespace

static_vertex_stream::static_vertex_stream(graphics_device& device,
    attribute_collection const& vertex_attributes)
{
    // Build vertex declaration from semantics, and corresponding attribute
    // vector.

    // Build explicit attributes first. ie POSITION0
    std::shared_ptr<source::vertex_attribute const> position_source
        = vertex_attributes.attribute(semantic("POSITION"));

    m_declaration.push_back(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0);

    std::vector<std::shared_ptr<source::vertex_attribute const> > attributes;
    attributes.push_back(position_source);

    // Then implicit attributes.
    for (attribute_collection::const_iterator i = vertex_attributes.begin();
        i != vertex_attributes.end(); ++i)
    {
        semantic const& semantic = i->first;
        std::shared_ptr<source::vertex_attribute> attribute = i->second;

        // Skip explicit attributes
        if (semantic.text() == "POSITION0")
            continue;

        D3DDECLTYPE decl_type = declaration_type(
            attribute->element_type(), attribute->element_count());
        D3DDECLUSAGE usage = d3d9_usage(semantic.name());
        BYTE index = static_cast<BYTE>(semantic.index());
        m_declaration.push_back(decl_type, usage, index);

        attributes.push_back(attribute);
    }

    // Build vertex buffer.

    m_vertex_count = position_source->count();

    DWORD bytes = static_cast<DWORD>(m_vertex_count * m_declaration.stride());

    m_buffer.reset(new vertex_buffer(device, bytes, D3DUSAGE_WRITEONLY, 0,
        D3DPOOL_DEFAULT));

    vertex_buffer_lock lock(m_buffer->native(), 0, bytes, 0);

    char* data = lock.data<char>();

    std::vector<D3DVERTEXELEMENT9> const& elements = m_declaration.elements();

    // For each vertex, write all elements.
    for (size_t i = 0; i < m_vertex_count; ++i)
    {
        for (size_t j = 0; j < elements.size(); ++j)
        {
            size_t bytes_written = write_to(data, attributes[j]->at(i),
                static_cast<D3DDECLTYPE>(elements[j].Type));

            data += bytes_written;
        }
    }
}

static_vertex_stream::static_vertex_stream(
    vertex_stream_declaration const& declaration,
    std::shared_ptr<vertex_buffer> buffer, size_t vertex_count)
:   m_declaration(declaration)
,   m_buffer(buffer)
,   m_vertex_count(vertex_count)
{
}


void static_vertex_stream::use(int stream_number) const
{
    UINT offset_in_bytes = 0;
    UINT stride = static_cast<UINT>(m_declaration.stride());

    set_stream_source(m_buffer->device(), static_cast<UINT>(stream_number),
        m_buffer->native(), offset_in_bytes, stride);
}

vertex_stream_declaration const& static_vertex_stream::declaration() const
{
    return m_declaration;
}

size_t static_vertex_stream::vertex_count() const
{
    return m_vertex_count;
}

//

void write_static_vertex_stream_dump(
    attribute_collection const& vertex_attributes, output_stream& output)
{
    vertex_stream_declaration declaration
        = create_vertex_stream_declaration(vertex_attributes);

    std::vector<std::shared_ptr<source::vertex_attribute const> > attributes
        = create_attributes(vertex_attributes);

    write_vertex_stream_declaration_dump(declaration, output);

    size_t vertex_count = attributes[0]->count();
    pt::write(output, uint32(vertex_count));

    size_t bytes = vertex_count * declaration.stride();
    std::vector<uint8> data_block;
    data_block.resize(bytes);

    char* data = reinterpret_cast<char*>(&data_block[0]);

    std::vector<D3DVERTEXELEMENT9> const& elements = declaration.elements();

    // For each vertex, write all elements.
    for (size_t i = 0; i < vertex_count; ++i)
    {
        for (size_t j = 0; j < elements.size(); ++j)
        {
            size_t bytes_written = write_to(data, attributes[j]->at(i),
                static_cast<D3DDECLTYPE>(elements[j].Type));

            data += bytes_written;
        }
    }
    output.write(&data_block[0], bytes);
}

std::shared_ptr<static_vertex_stream> read_static_vertex_stream_dump(
    graphics_device& device, input_stream& input)
{
    vertex_stream_declaration declaration
        = read_vertex_stream_declaration_dump(0, input);

    size_t vertex_count = pt::read<uint32>(input);
    //DWORD bytes = vertex_count * declaration.stride();

    DWORD bytes = static_cast<DWORD>(vertex_count * declaration.stride());

    std::shared_ptr<vertex_buffer> buffer(
        new vertex_buffer(device, bytes, D3DUSAGE_WRITEONLY, 0,
        D3DPOOL_DEFAULT));

    vertex_buffer_lock lock(buffer->native(), 0, bytes, 0);

    char* data = lock.data<char>();

    input.read(data, bytes);

    return std::make_shared<static_vertex_stream>(
        declaration, buffer, vertex_count);
}

}}} // namespace pt::d3d9::rendering
