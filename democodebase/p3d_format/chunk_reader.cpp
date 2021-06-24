#include "chunk_reader.h"

namespace p3d_format {

chunk_reader::chunk_reader(std::shared_ptr<data_input_stream> stream)
: m_stream(stream)
, m_start_pos(stream->position())
, m_parent(0)
{
    //size_t chunk_header_length = sizeof(uint32) + sizeof(int32);
    m_id = read<uint32>();
    m_length = read<int32>();
    assert(m_length > 0);
}

chunk_reader::chunk_reader(chunk_reader& parent)
: m_stream(shared_data_input_stream_with_shared_content_from(*parent.m_stream))
, m_start_pos(parent.m_stream->position())
, m_parent(&parent)
{
    m_stream->seek(m_start_pos);
    m_id = read<uint32>();
    m_length = read<int32>();
    //assert(m_length < 10 * 1024 * 1024);  // 10Mb
}

uint32 chunk_reader::id() const
{
    return m_id;
}

bool chunk_reader::end() const
{
    return m_stream->position() == m_length;
}

void chunk_reader::skip()
{
    size_t bytes_from_start = m_stream->position() - m_start_pos;
    size_t bytes_to_end = m_length - bytes_from_start;

    advance(bytes_to_end);
}

// Called when child chunks are read or skipped.
void chunk_reader::advance(size_t bytes)
{
    m_stream->seek(m_stream->position() + bytes);

    assert(m_stream->position() - m_start_pos <= m_length);

    if (m_parent)
        m_parent->advance(bytes);
}

} // namespace p3d_format
