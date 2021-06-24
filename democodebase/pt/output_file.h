#pragma once
#ifndef PT_OUTPUT_FILE_H
#define PT_OUTPUT_FILE_H

#include "stream_fwd.h"
#include "path_fwd.h"

namespace pt {

class output_file
{
public:
    output_file(pt::path const& path);
    output_file(pt::path const& path, file_type const& type, file_openmode mode);
    ~output_file();

    void close();
    size_t current_position() const;
    void seek_position(size_t byte_offset);
    size_t size() const;
    void write(void const* data, size_t bytes);

private: // noncopyable
    output_file(output_file const&);
    output_file const& operator=(output_file const&);

private: // data members
    class impl;
    impl* m_impl;
};

file_openmode default_output_mode();

} // namespace pt

#endif
