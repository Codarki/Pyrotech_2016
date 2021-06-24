#ifndef PT_P3D_FORMAT_CHUNK_WRITER_H
#define PT_P3D_FORMAT_CHUNK_WRITER_H

#include "pt/boost/noncopyable.h"
#include "id.h"
#include "pt/basic_types.h"
#include "pt/file_output_stream.h"

namespace p3d_format {

using namespace pt;

/// Writer for single chunk.
class chunk_writer : boost::noncopyable
{
public:
    chunk_writer(file_output_stream& stream, chunk_id id);
    ~chunk_writer();

    template<typename T>
    void write(T const& value);

private: // data members
    file_output_stream& m_stream;
    chunk_id m_id;
    size_t m_length;
    size_t m_start_position;
};

template<typename T>
void chunk_writer::write(T const& value)
{
    pt::write(m_stream, value);
}

} // namespace p3d_format

#endif
