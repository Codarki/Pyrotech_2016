#include "vertex_streams.h"
#include "enum_utility.h"
#include "stream_source.h"
#include "pt/basic_types.h"
#include "pt/any_error.h"
#include "pt/boost/cast.h"
#include "pt/math/rounding.h"

namespace pt { namespace d3d9 {

vertex_streams::vertex_streams(graphics_device& device)
:   m_declaration(device)
{
}

void vertex_streams::use() const
{
    assert(m_declaration.valid());
    m_declaration.use();

    for (size_t i = 0; i < m_streams.size(); ++i)
    {
        m_streams[i]->use(static_cast<int>(i));
    }
}

void vertex_streams::add(std::shared_ptr<stream_source> source)
{
    assert(source);

    size_t stream_index = m_streams.size();
    m_streams.push_back(source);

    m_declaration.stream(stream_index) = source->declaration();
}

void vertex_streams::finalize()
{
    assert(!m_declaration.valid());
    m_declaration.finalize();
    assert(m_declaration.valid());
}

size_t vertex_streams::vertex_count() const
{
    assert(!m_streams.empty());
    return m_streams[0]->vertex_count();
}

namespace {
    template<typename T>
    void write_integer_element(void* dst, double value)
    {
        T* dest = reinterpret_cast<T*>(dst);
        *dest = numeric_cast<T>(math::round(value));
    }
    void write_float_element(void* dst, double value)
    {
        float* dest = reinterpret_cast<float*>(dst);
        *dest = static_cast<float>(value);
    }

    void write_element(d3d9::vertex_element_type type, void* dst, double value)
    {
        if (type == element_int8)
            write_integer_element<int8>(dst, value);
        else if (type == element_uint8)
            write_integer_element<uint8>(dst, value);
        else if (type == element_int16)
            write_integer_element<int16>(dst, value);
        else if (type == element_uint16)
            write_integer_element<uint16>(dst, value);
        else if (type == element_float)
            write_float_element(dst, value);
        else
            throw any_error("umm");
    }
} // anonymous namespace

size_t write_to(char* dst, vector4d const& value, D3DDECLTYPE type)
{
    size_t element_count = declaration_element_count(type);
    size_t element_size = declaration_element_size_in_bytes(type);
    size_t bytes = 0;

    for (size_t i = 0; i < element_count; ++i)
    {
        write_element(element_type(type), dst, value[i]);
        dst += element_size;
        bytes += element_size;
    }

    return bytes;
}

}} // namespace pt::d3d9
