#include "win32_error.h"
#include "pt/debug_only.h"
#include "pt/win32.h"
#include "pt/string_wrapper.h"
#include "pt/format.h"
#include <cassert>

namespace pt { namespace win32 {

namespace {
    string_wrapper create_error_text(std::string const& text)
    {
        std::string error_text;

        DWORD error_code = GetLastError();

        if (error_code == 0)
        {
            error_text = (format("Win32 %1% error: Unspecified error")
                % text).move_string();
        }
        else
        {
            DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM;

            // Returning specific language has possibility of failing. We dont
            // want that inside of exceptions.
            DWORD language_id = 0; //MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)

            // Lets assume this is ANSI and TCHAR is char.
            char* message_buffer = 0;

            PT_DEBUG_ONLY(DWORD result = )
                FormatMessage(flags, 0, error_code, language_id,
                reinterpret_cast<LPTSTR>(&message_buffer), 0, 0);

            assert(result);

            error_text = (format("Win32 %1% error %2%: %3%")
                % text % error_code % std::string(message_buffer)).move_string();

            LocalFree(message_buffer);
        }

        return move(error_text);
    }
} // anonymous namespace

// TODO not sure if this works on unicode
win32_error::win32_error(string_wrapper const& text)
{

    set_text(create_error_text(text.string()));
}

win32_error::win32_error(format const& fmt)
{

    set_text(create_error_text(fmt.string()));
}

win32_error::win32_error(char const* text)
{
    set_text(create_error_text(text));
}

win32_error::win32_error(win32_error const& other)
: exceptions::contextual_exception(other)
{
}

win32_error::~win32_error()
{
}

}} // namespace pt::win32
