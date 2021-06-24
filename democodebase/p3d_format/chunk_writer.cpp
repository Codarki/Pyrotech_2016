#include "chunk_writer.h"
#include "pt/log.h"
#include "pt/std/cassert.h"

namespace p3d_format {

chunk_writer::chunk_writer(file_output_stream& stream,
    chunk_id id)
:   m_stream(stream)
,   m_start_position(stream.current_pos())
,   m_id(id)
{
    pt::write(m_stream, static_cast<uint32>(m_id));
    pt::write(m_stream, static_cast<uint32>(0)); // chunk_len unkown at this point.
}

chunk_writer::~chunk_writer()
{
    try
    {
        // write back the size of this chunk
        size_t current_pos = m_stream.current_pos();

        m_stream.seek(m_start_position + sizeof(uint32));
        size_t chunk_size = current_pos - m_start_position;
        assert(chunk_size > 0);
        pt::write(m_stream, static_cast<uint32>(chunk_size));

        m_stream.seek(current_pos);
    }
    catch(...)
    {
        // Implement better error reporting.
        assert(false);
        log("exception in destructor!");
    }
}

} // namespace p3d_format
