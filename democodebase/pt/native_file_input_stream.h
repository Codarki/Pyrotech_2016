#pragma once
#ifndef PT_NATIVE_FILE_INPUT_STREAM_H
#define PT_NATIVE_FILE_INPUT_STREAM_H

#include "bounded_input_stream.h"
#include "path_fwd.h"
#include "pt/std/memory.h"

namespace pt {

class input_file;

/// Input stream from a file.
class native_file_input_stream : public bounded_input_stream
{
public:
    /// Opens \a path for reading.
    native_file_input_stream(path const& path);
    /// Opens \a path for reading.
    //native_file_input_stream(path const& path, std::ios_base::openmode mode);
    ~native_file_input_stream();

public: // interface required by input_stream
    void read(void* data, size_t bytes);

public: // interface required by bounded_input_stream
    void seek(size_t offset);
    size_t position() const;
    size_t size() const;

private: // noncopyable
    native_file_input_stream(native_file_input_stream const&);
    native_file_input_stream const& operator=(native_file_input_stream const&);

private: // data members
    std::unique_ptr<input_file> m_file;
};

} // namespace pt

#endif
