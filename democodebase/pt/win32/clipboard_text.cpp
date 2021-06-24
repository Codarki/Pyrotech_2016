#include "clipboard_text.h"
#include "utility.h"
#include "win32_error.h"

namespace pt { namespace win32 {

clipboard_text::clipboard_text()
{
}

clipboard_text::clipboard_text(std::string const& text)
:   m_text(text)
{
}

std::string const& clipboard_text::text() const
{
    return m_text;
}

void clipboard_text::copy() const
{
    //size_t characters = 0;
    SIZE_T bytes = (m_text.length() + 1) * sizeof(TCHAR);

    // \todo Implement scoped GlobalAlloc-GlobalFree if any of these ever
    // fails. SetClipboardData() will assume ownership of the allocated
    // memory.

    // Allocate a global memory object for the text.
    HGLOBAL mem_handle = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (mem_handle == NULL)
        throw win32_error("GlobalAlloc");

    // Lock the handle and copy the text to the buffer.
    {
        scoped_global_lock lock(mem_handle);
        memcpy(lock.data<void>(), m_text.c_str(), m_text.length());
        lock.data<char>()[m_text.length()] = 0;
    }

    UINT format = CF_TEXT;
    HANDLE data_handle = SetClipboardData(format, mem_handle);
    if (!data_handle)
        throw win32_error("SetClipboardData");
}

void clipboard_text::paste()
{
    HANDLE mem_handle = GetClipboardData(CF_TEXT); 
    if (mem_handle == NULL)
        throw win32_error("GetClipboardData");

    scoped_global_lock lock(mem_handle);
    m_text = lock.data<char>();
}

bool clipboard_text::is_available() const
{
    // Crappy API.. To get extended error information, call GetLastError.
    BOOL result = IsClipboardFormatAvailable(CF_TEXT);
    return result != 0;
}

}} // namespace pt::win32
