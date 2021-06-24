#pragma once
#ifndef PT_DATA_STREAM_CONTENT_H
#define PT_DATA_STREAM_CONTENT_H

namespace pt {

class bounded_input_stream;

/// Block of raw data.
class data_stream_content
{
public:
    // Allocates data pool of given \a bytes.
    data_stream_content(size_t bytes);
    // Creates a copy of \a x.
    data_stream_content(bounded_input_stream& x);
    // Creates a data stream from reading \a bytes from \a offset from \a x.
    data_stream_content(bounded_input_stream& x, size_t offset, size_t bytes);
    ~data_stream_content();

    void* data();

    /// \return Size in bytes of this stream content.
    size_t size() const;
    /// Reads \a bytes from \a byte_offset into \a data.
    void read(size_t byte_offset, void* data, size_t bytes) const;
    /// Writes \a bytes from \a data into \a byte_offset.
    void write(size_t byte_offset, void const* data, size_t bytes);

private: // noncopyable
    data_stream_content(data_stream_content const&);
    data_stream_content const& operator=(data_stream_content const&);

private: // data members
    typedef unsigned char byte;
    byte* m_bytes;
    size_t m_size;
};

} // namespace pt

#endif
