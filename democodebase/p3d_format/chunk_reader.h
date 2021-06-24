#ifndef PT_P3D_FORMAT_CHUNK_READER_H
#define PT_P3D_FORMAT_CHUNK_READER_H

#include "pt/basic_types.h"
#include "pt/data_input_stream.h"
#include "pt/std/memory.h"
#include "pt/std/cassert.h"

namespace p3d_format {

using namespace pt;

/// Reader for single chunk.
class chunk_reader
{
public:
    /// Creates a root chunk reader from current position of \a stream.
    chunk_reader(std::shared_ptr<data_input_stream> stream);

    /// Creates reader for child chunk of \a parent. \a parent is advanced
    /// in sync with this chunk.
    chunk_reader(chunk_reader& parent);

    uint32 id() const;

    template<typename T>
    T read();

    template<typename T>
    void read_array(size_t count, T* destination);

    bool end() const;
    void skip();

private:
    void advance(size_t bytes);

    // noncopyable
    //chunk_reader(chunk_reader const&);
    chunk_reader const& operator=(chunk_reader const&);

private: // data members
    std::shared_ptr<data_input_stream> m_stream;
    chunk_reader* m_parent;
    uint32 m_id;
    size_t m_length;
    size_t m_start_pos;
};

template<typename T>
inline T chunk_reader::read()
{
    // sizeof(T) does not work, it might be std::string, vector3f or such.
    size_t cur_pos = m_stream->position();

    T value;
    pt::read(*m_stream, value);

    assert(m_length == 0 || (m_stream->position() - m_start_pos <= m_length));

    size_t bytes_read = m_stream->position() - cur_pos;

    if (m_parent)
        m_parent->advance(bytes_read);

    return value;
}

template<typename T>
inline void chunk_reader::read_array(size_t count, T* destination)
{
    // sizeof(T) does not work, it might be std::string, vector3f or such.
    size_t cur_pos = m_stream->position();

    pt::read_array(*m_stream, count, destination);

    assert(m_stream->position() - m_start_pos <= m_length);

    size_t bytes_read = m_stream->position() - cur_pos;

    if (m_parent)
        m_parent->advance(bytes_read);
}

} // namespace p3d_format

#endif
