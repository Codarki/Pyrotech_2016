#include "input_file.h"
#include "path.h"
#include "exceptions/file_error.h"
#include "pt/std/cassert.h"
#include <fstream>

namespace pt {

class input_file::impl
{
public:
    impl(pt::path const& path)
    : m_path(path)
    {
        std::ios_base::openmode mode = std::ios_base::in;
        mode |= std::ios_base::binary;

        m_input.open(m_path.string().c_str(), mode);

        if (!m_input.is_open())
        {
            throw exceptions::file_error(m_path, "Couldn't open file for reading");
        }

        m_position = 0;

        // Get size.
        {
            std::streampos current_position = m_input.tellg();

            m_input.seekg(0, std::ifstream::end);
            m_size = static_cast<size_t>(m_input.tellg());

            m_input.seekg(current_position, std::ifstream::beg);
        }
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
            //log(std::string("exception in destructor: ") + e.what());
            assert(false);
        }
    }

    void close()
    {
        if (m_input.is_open())
        {
            m_input.close();

            if (m_input.fail())
                throw exceptions::file_error(m_path, "Couldn't close file");
        }
    }

    size_t current_position() const
    {
        return m_position;
    }

    void seek_position(size_t byte_offset)
    {
        if (byte_offset != m_position)
        {
            if (byte_offset > m_size)
            {
                throw exceptions::file_error(m_path,
                    "Trying to seek out of bounds");
            }

            if (!m_input.is_open())
                throw exceptions::file_error(m_path, "file not open");

            m_input.seekg(static_cast<std::streamoff>(byte_offset),
                std::ifstream::beg);

            m_position = byte_offset;
        }
    }

    size_t size() const
    {
        return m_size;
    }

    void read(void* data, size_t bytes) const
    {
        assert(m_position + bytes <= m_size && "Trying to read out of bounds");
        if (m_position + bytes > m_size)
            throw exceptions::file_error(m_path, "Trying to read out of bounds");

        m_input.read(
            reinterpret_cast<char*>(data),
            static_cast<std::streamsize>(bytes)
        );

        m_position += bytes;
    }

private: // data members
    mutable std::ifstream m_input;
    pt::path m_path;
    mutable size_t m_position;
    size_t m_size;
};

input_file::input_file(pt::path const& path)
: m_impl(new impl(path))
{
}

input_file::~input_file()
{
    delete m_impl;
}

void input_file::close()
{
    m_impl->close();
}

size_t input_file::current_position() const
{
    return m_impl->current_position();
}

void input_file::seek_position(size_t byte_offset)
{
    return m_impl->seek_position(byte_offset);
}

size_t input_file::size() const
{
    return m_impl->size();
}

void input_file::read(void* data, size_t bytes) const
{
    m_impl->read(data, bytes);
}

} // namespace pt
