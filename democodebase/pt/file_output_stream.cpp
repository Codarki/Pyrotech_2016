#include "file_output_stream.h"
#include "output_file.h"
#include "file_openmode.h"

namespace pt {

class file_output_stream::impl
{
public:
    impl(pt::path const& path)
    : m_file(path)
    {
    }

    impl(pt::path const& path, file_type const& type, file_openmode mode)
    : m_file(path, type, mode)
    {
    }

    void write(void const* data, size_t bytes)
    {
        m_file.write(data, bytes);
    }

    void seek(size_t offset)
    {
        m_file.seek_position(offset);
    }

    size_t current_pos()
    {
        return m_file.current_position();
    }

private:
    output_file m_file;
};

// file_output_stream

file_output_stream::file_output_stream(pt::path const& path)
: m_impl(new impl(path))
{
}

file_output_stream::file_output_stream(pt::path const& path,
    file_type const& type, file_openmode mode)
: m_impl(new impl(path, type, mode))
{
}

file_output_stream::~file_output_stream()
{
    delete m_impl;
}

void file_output_stream::write(void const* data, size_t bytes)
{
    m_impl->write(data, bytes);
}

void file_output_stream::seek(size_t offset)
{
    return m_impl->seek(offset);
}

size_t file_output_stream::current_pos()
{
    return m_impl->current_pos();
}

} // namespace pt
