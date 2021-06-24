#ifndef PT_WIN32_WINDOW_H
#define PT_WIN32_WINDOW_H

#include "pt/defs.h"
#include "pt/win32.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace win32 {

class window_class;

/// Win32 window.
/// Each window is a member of a particular window class. All windows
/// belonging to the same class use the same default window procedure to
/// process its messages.
///
class window
{
public:
    /// Constructs a new window. Issues WM_CREATE message before returns.
    ///
    /// \param name Name.
    /// \param style Style for the window.
    /// \param x For an overlapped or pop-up, initial x-coordinate of the
    ///     window's upper-left corner, in screen coordinates. For a child
    ///     window, \a x is the x-coordinate of the upper-left corner of the
    ///     window relative to the upper-left corner of the parent window's
    ///     client area.
    /// \param y Same functionality as \a x parameter.
    /// \param width Specifies the width, in device units, of the window.
    /// \param height Specifies the height, in device units, of the window.
    /// \param parent Parent window.
    /// \param menu Handle to a menu, or specifies a child-window identifier.
    /// \param window_class Window class whose window procedure is used.
    ///
    window(std::string const& name, DWORD style, int x, int y, int width,
        int height, window* parent, HMENU menu,
        std::shared_ptr<window_class> const& window_class);
    /// \overload
    /// \a rect encompasses the size parameters from previous function.
    window(std::string const& name, DWORD style, RECT const& rect,
        window* parent, HMENU menu,
        std::shared_ptr<window_class> const& window_class);

    /// Destroys the window. Issues WM_DESTROY message before returns.
    ~window();

    HWND handle() const;

private: // data members
    std::shared_ptr<window_class> m_window_class;
    HWND m_handle;
};

/// Creates default windows style matching the parameters.
/// \relatesalso win32::window
DWORD create_window_style(bool is_child, bool disabled, bool dialog);

}} // namespace pt::win32

#endif
