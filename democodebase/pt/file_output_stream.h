#ifndef PT_FILE_OUTPUT_STREAM_H
#define PT_FILE_OUTPUT_STREAM_H

#include "output_stream.h"
#include "stream_fwd.h"
#include "path_fwd.h"

namespace pt {

/// Output stream to a file.
class file_output_stream : public output_stream
{
public:
    /// Opens \a path for writing.
    file_output_stream(pt::path const& path);
    file_output_stream(pt::path const& path, file_type const& type,
        file_openmode mode);
    ~file_output_stream();

public: // interface required by output_stream
    void write(void const* data, size_t bytes);

public: // \todo bounded_output_stream
    void seek(size_t offset);
    size_t current_pos();

private: // noncopyable
    file_output_stream(file_output_stream const&);
    file_output_stream const& operator=(file_output_stream const&);

private: // data members
    class impl;
    impl* m_impl;
};

} // namespace pt

#endif
