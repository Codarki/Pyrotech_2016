#include "clipboard.h"
#include "win32_error.h"
#include "pt/log.h"
#include "pt/std/cassert.h"

namespace pt { namespace win32 {

namespace {
    /// Keeps clipboard open in the scope. Calls OpenClipboard() and
    /// CloseClipboard().
    class scoped_clipboard
    {
    public:
        scoped_clipboard(HWND handle)
        {
            assert(handle);

            if (OpenClipboard(handle) == 0)
                throw win32_error("OpenClipboard");
        }
        ~scoped_clipboard()
        {
            if (CloseClipboard() == 0)
            {
                assert(false);
                log("Error: exception in ~scoped_clipboard");
                //throw win32_error("CloseClipboard");
            }
        }
    };
} // anonymous namespace

clipboard::clipboard(HWND handle)
:   m_handle(handle)
{
}

clipboard::clipboard(HWND handle, std::shared_ptr<format> format)
:   m_handle(handle)
{
    add(format);
}

void clipboard::add(std::shared_ptr<format> format)
{
    assert(format);
    m_formats.push_back(format);
}

void clipboard::copy() const
{
    scoped_clipboard current_clipboard(m_handle);

    if (EmptyClipboard() == 0)
        throw win32_error("EmptyClipboard");

    for (formats::const_iterator i = m_formats.begin(); i != m_formats.end();
        ++i)
    {
        (*i)->copy();
    }
}

void clipboard::paste()
{
    scoped_clipboard current_clipboard(m_handle);

    for (formats::const_iterator i = m_formats.begin(); i != m_formats.end();
        ++i)
    {
        assert(*i);
        format& format = *(*i);

        if (format.is_available())
            format.paste();
    }
}

}} // namespace pt::win32
