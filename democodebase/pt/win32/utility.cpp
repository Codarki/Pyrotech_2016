#include "utility.h"
#include "win32_error.h"

namespace pt { namespace win32 {

// scoped_paint

scoped_paint::scoped_paint(HWND handle)
:   m_handle(handle)
{
    m_hdc = BeginPaint(m_handle, &m_ps);
    if (!m_hdc)
        throw win32_error("BeginPaint");
}
scoped_paint::~scoped_paint()
{
    EndPaint(m_handle, &m_ps);
}

// scoped_global_lock

scoped_global_lock::scoped_global_lock(HANDLE handle)
:   m_handle(handle)
{
    m_ptr = GlobalLock(m_handle);
    if (!m_ptr)
        throw win32_error("GlobalLock");
}

scoped_global_lock::~scoped_global_lock()
{
    // Shitty API..

    // If the memory object is still locked after decrementing the lock
    // count, the return value is a nonzero value.
    // If the function fails, the return value is zero. To get extended
    // error information, call GetLastError. If GetLastError returns
    // NO_ERROR, the memory object is unlocked.

    //if (GlobalUnlock(m_handle) == 0)
    //    throw win32_error("GlobalUnlock");

    GlobalUnlock(m_handle);
}

// free functions

box2i box_from(RECT const& rect)
{
    vector2i min_position(rect.left, rect.top);
    vector2i max_position(rect.right, rect.bottom);
    return box_from_min_max(min_position, max_position);
}

RECT rect_from(box2i const& box)
{
    RECT rect;
    rect.left = min_axis(box, 0);
    rect.top = min_axis(box, 1);
    rect.right = max_axis(box, 0);
    rect.bottom = max_axis(box, 1);
    return rect;
}

RECT rect_from(vector2i const& min, vector2i const& max)
{
    RECT rect;
    rect.left = min[0];
    rect.top = min[1];
    rect.right = max[0];
    rect.bottom = max[1];
    return rect;
}

}} // namespace pt::win32
