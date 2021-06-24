#include "output_file.h"
#include "path.h"
#include "file_openmode.h"
#include "file_type.h"
#include "exceptions/file_error.h"
#include "pt/std/cassert.h"
#include <fstream>

namespace pt {

namespace {
    std::ios_base::openmode default_std_output_mode()
    {
        return std::ios_base::out | std::ios_base::binary
            | std::ios_base::trunc;
    }

    void open(pt::path const& path, std::ios_base::openmode mode,
        std::ofstream& output)
    {
        output.open(path.string().c_str(), mode);

        if (!output.is_open())
        {
            throw exceptions::file_error(
                path, "Couldn't open file for writing");
        }
    }

    std::ios_base::openmode std_openmode_from_file_openmode(
        file_type const& type, file_openmode mode)
    {
        std::ios_base::openmode result = 0;

        if (mode & pt::openmode::in)
            result |= std::ios_base::in;

        if (mode & pt::openmode::out)
            result |= std::ios_base::out;

        if (mode & pt::openmode::ate)
            result |= std::ios_base::ate;

        if (mode & pt::openmode::append)
            result |= std::ios_base::app;

        if (mode & pt::openmode::truncate)
            result |= std::ios_base::trunc;

        if (type.is_binary())
            result |= std::ios_base::binary;

        return result;
    }

} // anonymous namespace

class output_file::impl
{
public:
    impl(pt::path const& path)
    : m_path(path)
    {
        open(m_path, default_std_output_mode(), m_output);
    }

    impl(pt::path const& path, file_type const& type, file_openmode mode)
    : m_path(path)
    {
        mode |= pt::openmode::out;

        open(m_path, std_openmode_from_file_openmode(type, mode), m_output);
    }

    ~impl()
    {
        try
        {
            close();
        }
        catch(std::exception& e)
        {
            std::string text = std::string("exception in destructor: ") + e.what();
            assert(false);
        }
    }

    void close()
    {
        if (m_output.is_open())
        {
            m_output.close();

            if (m_output.fail())
                throw exceptions::file_error(m_path, "Couldn't close file");
        }
    }

    size_t current_position() const
    {
        assert(m_output.is_open());
        return static_cast<size_t>(m_output.tellp());
    }

    void seek_position(size_t byte_offset)
    {
        assert(byte_offset <= size());

        if (byte_offset > size())
        {
            throw exceptions::file_error(m_path, "Trying to seek out of bounds");
        }

        if (!m_output.is_open())
            throw exceptions::file_error(m_path, "file not open");

        m_output.seekp(static_cast<std::streamoff>(byte_offset),
            std::ofstream::beg);
    }

    size_t size() const
    {
        if (!m_output.is_open())
            throw exceptions::file_error(m_path, "file not open");

        std::streampos orig_pos = m_output.tellp();
        m_output.seekp(0, std::ofstream::end);
        size_t size = static_cast<size_t>(m_output.tellp());
        m_output.seekp(orig_pos, std::ofstream::end);

        return size;
    }

    void write(void const* data, size_t bytes)
    {
        m_output.write(
            reinterpret_cast<char const*>(data),
            static_cast<std::streamsize>(bytes)
        );
        assert(m_output.good());
    }

private: // data members
    mutable std::ofstream m_output;
    pt::path m_path;
    mutable size_t m_position;
    size_t m_size;
};

output_file::output_file(pt::path const& path)
: m_impl(new impl(path))
{
}

output_file::output_file(pt::path const& path, file_type const& type, file_openmode mode)
: m_impl(new impl(path, type, mode))
{
}

output_file::~output_file()
{
    delete m_impl;
}

void output_file::close()
{
    m_impl->close();
}

size_t output_file::current_position() const
{
    return m_impl->current_position();
}

void output_file::seek_position(size_t byte_offset)
{
    return m_impl->seek_position(byte_offset);
}

size_t output_file::size() const
{
    return m_impl->size();
}

void output_file::write(void const* data, size_t bytes)
{
    m_impl->write(data, bytes);
}

// free functions

file_openmode default_output_mode()
{
    return pt::openmode::out | pt::openmode::truncate;
}

} // namespace pt
