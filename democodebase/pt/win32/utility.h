#ifndef PT_WIN32_UTILITY_H
#define PT_WIN32_UTILITY_H

#include "../defs.h"
#include "../box.h"
#include "../vector.h"
#include "../win32.h"

namespace pt { namespace win32 {

/// This class should only be used in response to WM_PAINT message. Calls
/// BeginPaint() and EndPaint().
class scoped_paint
{
public:
    /// If the update region is marked for erasing, uses the background brush
    /// or sends a WM_ERASEBKGND message to the window to erase the background
    /// of the update region before returning.
    scoped_paint(HWND handle);
    ~scoped_paint();

private: // data members
    HWND m_handle;
    HDC m_hdc;
    PAINTSTRUCT m_ps;
};

/// Performs GlobalLock() and GlobalUnlock().
class scoped_global_lock
{
public:
    scoped_global_lock(HANDLE handle);
    ~scoped_global_lock();

    template<typename T>
    T* data() const;

private: // data members
    void* m_ptr;
    HANDLE m_handle;
};

box2i box_from(RECT const& rect);
RECT rect_from(box2i const& box);
RECT rect_from(vector2i const& min, vector2i const& max);

// todo move to .inl
template<typename T>
T* scoped_global_lock::data() const
{
    return reinterpret_cast<T*>(m_ptr);
}

}} // namespace pt::win32

#endif
