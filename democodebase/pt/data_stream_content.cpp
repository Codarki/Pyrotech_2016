#include "data_stream_content.h"
#include "bounded_input_stream.h"
#include "std/cassert.h"
#include <algorithm>

namespace pt {

// data_stream_content

data_stream_content::data_stream_content(size_t bytes)
: m_size(bytes)
{
    m_bytes = new byte[m_size];
}

data_stream_content::data_stream_content(bounded_input_stream& x)
: m_size(x.size())
{
    m_bytes = new byte[m_size];
    x.seek(0);
    x.read(m_bytes, m_size);
}

data_stream_content::data_stream_content(bounded_input_stream& x,
    size_t offset, size_t bytes)
: m_size(bytes)
{
    m_bytes = new byte[m_size];
    x.seek(offset);
    x.read(m_bytes, m_size);
}

data_stream_content::~data_stream_content()
{
    delete [] m_bytes;
}

void* data_stream_content::data()
{
    return m_bytes;
}

size_t data_stream_content::size() const
{
    return m_size;
}

void data_stream_content::read(size_t byte_offset, void* data,
    size_t bytes) const
{
    assert(byte_offset + bytes <= size());

    byte const* first = m_bytes + byte_offset;
    byte const* last = first + bytes;

    std::copy(first, last, reinterpret_cast<byte*>(data));
}

void data_stream_content::write(size_t byte_offset, void const* data,
    size_t bytes)
{
    assert(byte_offset + bytes < size());

    byte const* first = reinterpret_cast<byte const*>(data);
    byte const* last = first + bytes;

    std::copy(first, last, &m_bytes[byte_offset]);
}

} // namespace pt
