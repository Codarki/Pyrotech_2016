#pragma once
#ifndef PT_FILE_INPUT_STREAM_H
#define PT_FILE_INPUT_STREAM_H

#include "bounded_input_stream.h"
#include "path_fwd.h"
#include "std/memory.h"

namespace pt {

/// Input stream from a file.
class file_input_stream : public bounded_input_stream
{
public:
    /// Opens \a path for reading.
    file_input_stream(path const& path);
    ///// Opens \a path for reading.
    //file_input_stream(path const& path, std::ios_base::openmode mode);

public: // interface required by input_stream
    void read(void* data, size_t bytes);

public: // interface required by bounded_input_stream
    void seek(size_t offset);
    size_t position() const;
    size_t size() const;

private: // helper functions
    void find_from_archives(path const& path);

private: // data members
    std::unique_ptr<bounded_input_stream> m_stream;
};

bool immutable_file_exists(path const& path);

} // namespace pt

#endif
