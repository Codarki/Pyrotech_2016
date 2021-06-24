#pragma once
#ifndef PT_INPUT_FILE_H
#define PT_INPUT_FILE_H

#include "path_fwd.h"

namespace pt {

class input_file
{
public:
    input_file(pt::path const& path);
    ~input_file();

    void close();
    size_t current_position() const;
    void seek_position(size_t byte_offset);
    size_t size() const;
    void read(void* data, size_t bytes) const;

private: // noncopyable
    input_file(input_file const&);
    input_file const& operator=(input_file const&);

private: // data members
    class impl;
    impl* m_impl;
};

} // namespace pt

#endif
