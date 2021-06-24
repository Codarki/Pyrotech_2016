#include "native_c_file.h"
#include "pt/path.h"
#include "pt/file_openmode.h"
#include "pt/file_type.h"
#include "pt/exceptions/file_error.h"
#include "pt/std/cassert.h"
#include "pt/debug_only.h"
#include <cstdio>

namespace pt {

namespace {
    std::string get_error_text(errno_t code)
    {
        char error_message[256];
        PT_DEBUG_ONLY(errno_t result =) strerror_s(error_message, code);
        assert(result != 0);

        return std::string(error_message);
    }

    const char* read_binary = "rb";
    const char* write_binary = "wb";
    const char* append_binary = "ab";
    const char* read_text = "b";
    const char* write_text = "w";
    const char* append_text = "a";

    const char* c_mode_for(const file_type& type, file_openmode mode)
    {
        const char* c_mode = 0;

        if (type.is_binary())
        {
            if (mode == openmode::in)
                c_mode = read_binary;

            if (mode == openmode::out)
                c_mode = write_binary;

            if (mode & openmode::append)
                c_mode = append_binary;
        }
        else
        {
            if (mode == openmode::in)
                c_mode = read_text;

            if (mode == openmode::out)
                c_mode = write_text;

            if (mode & openmode::append)
                c_mode = append_text;
        }

        assert(c_mode != 0);

        return c_mode;
    }
} // anonymous

class native_c_file::impl
{
public:
    impl(const pt::path& path, const file_type& type, file_openmode mode)
    : m_path(path)
    {
        errno_t result = fopen_s(&m_native, m_path.string().c_str(),
            c_mode_for(type, mode));

        if (result != 0)
            throw pt::exceptions::file_error(m_path, get_error_text(result));
    }

    ~impl()
    {
        try
        {
            close();
        }
        catch (std::exception&)
        {
            assert(!"File close failed in destructor.");
            //std::abort();
        }
    }

    void close()
    {
        int result = fclose(m_native);
        if (result != 0)
            throw pt::exceptions::file_error(m_path, get_error_text(result));
    }

    void read(void* destination, size_t byte_count)
    {
        const size_t count = 1;
        const size_t elements_read
            = fread(destination, byte_count, count, m_native);

        if (elements_read != count)
        {
            if (::ferror(m_native) != 0)
            {
                throw pt::exceptions::native_file_error(m_path,
                    pt::exceptions::native_file_reading_failed);
            }

            if (::feof(m_native))
            {
                throw pt::exceptions::native_file_error(m_path,
                    pt::exceptions::native_file_reading_end_of_file);
            }

            throw pt::exceptions::native_file_error(m_path,
                pt::exceptions::native_file_reading_failed);
        }
    }

    void write(const void * destination, size_t byte_count)
    {
        const size_t count = 1;
        const size_t elements_written = fwrite(destination, byte_count, count, m_native);

        if (elements_written != count)
            throw pt::exceptions::file_error(m_path, get_error_text(elements_written));
    }

private: // data members
    FILE* m_native;
    pt::path m_path;
};

native_c_file::native_c_file(const pt::path& path, const file_type& type, file_openmode mode)
: m_impl(new impl(path, type, mode))
{
}

native_c_file::~native_c_file()
{
    delete m_impl;
}

} // namespace pt
