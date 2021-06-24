#include "file_error.h"
#include "path_file_context.h"
#include "pt/string_wrapper.h"
#include "pt/std/cassert.h"

namespace pt { namespace exceptions {

namespace {
    const char* error_text[] =
    {
        "Reading file failed.",
        "Reading file failed. End Of File error."
    };

    const size_t error_text_count = sizeof(error_text) / sizeof(error_text[0]);

    const char* text_for(native_file_error_code code)
    {
        assert(code < native_file_error_code_count);
        assert(native_file_error_code_count == error_text_count);

        return error_text[code];
    }
} // anonymous

file_error::file_error(path const& path, std::string const& text)
{
    set_text(text);
    attach_context(new path_file_context(path));
}

file_error::file_error(path const& path, format const& fmt)
{
    set_text(fmt);
    attach_context(new path_file_context(path));
}

native_file_error::native_file_error(const pt::path& path,
    native_file_error_code code)
{
    set_text(text_for(code));
    attach_context(new path_file_context(path));
}

}} // namespace pt::exceptions
