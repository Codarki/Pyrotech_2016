#include "window.h"
#include "window_class.h"
#include "win32_error.h"
#include "pt/std/cassert.h"

namespace pt { namespace win32 {
namespace {
    DWORD create_window_ex_style()
    {
        DWORD ex_style = 0;
        // Forces a top-level window onto the taskbar when the window is
        // visible.
        ex_style |= WS_EX_APPWINDOW;
        // Window has a border with a sunken edge.
        //ex_style |= WS_EX_CLIENTEDGE;

        return ex_style;
    }
} // anonymous namespace

window::window(std::string const& name, DWORD style, int x, int y, int width,
    int height, window* parent, HMENU menu,
    std::shared_ptr<window_class> const& window_class)
:   m_window_class(window_class)
{
    assert(m_window_class);

    // Lets not use this, looks hacky.
    LPVOID param = 0;

    m_handle = CreateWindowEx(
        create_window_ex_style(),
        reinterpret_cast<LPCTSTR>(m_window_class->atom()),
        name.c_str(),
        style, x, y,
        width, height,
        parent ? parent->handle() : 0,
        menu,
        m_window_class->instance(),
        param);

    if (m_handle == 0)
        throw win32_error("CreateWindowEx");
}

window::window(std::string const& name, DWORD style, RECT const& rect,
    window* parent, HMENU menu,
    std::shared_ptr<window_class> const& window_class)
:   m_window_class(window_class)
{
    assert(m_window_class);

    // Lets not use this, looks hacky.
    LPVOID param = 0;

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    m_handle = CreateWindowEx(
        create_window_ex_style(),
        reinterpret_cast<LPCTSTR>(m_window_class->atom()),
        name.c_str(),
        style, rect.left, rect.top,
        width, height,
        parent ? parent->handle() : 0,
        menu,
        m_window_class->instance(),
        param);

    if (m_handle == 0)
        throw win32_error("CreateWindowEx");
}

window::~window()
{
    //BOOL result =
		DestroyWindow(m_handle);
    //if (!result)
    //    throw win32_error("DestroyWindow");
    m_window_class.reset();
}

HWND window::handle() const
{
    return m_handle;
}

DWORD create_window_style(bool is_child, bool disabled, bool dialog)
{
    DWORD style = 0;
    // Window has a title bar (includes the WS_BORDER style). TODO lets not
    // force this (buttons for example.)
    style |= WS_CAPTION;
    // A window with WS_CHILD style cannot have a menu bar. This style cannot
    // be used with the WS_POPUP style.
    style |= is_child ? WS_CHILD : 0;
    // Excludes the area occupied by child windows when drawing occurs within
    // the parent window.
    style |= WS_CLIPCHILDREN;
    // Window that is initially disabled. (use EnableWindow())
    style |= disabled ? WS_DISABLED : 0;
    // Window that has a border of a style used with dialog boxes. A window
    // with this style cannot have a title bar.
    style |= dialog ? WS_DLGFRAME : 0;

    // TODO WS_GROUP
    // TODO WS_TABSTOP

    // Window that has a minimize & maximize button The WS_SYSMENU style must
    // also be specified. TODO lets not force these
    style |= WS_MINIMIZEBOX;
    style |= WS_MAXIMIZEBOX;
    // Window that has a window menu on its title bar. The WS_CAPTION style
    // must also be specified.
    style |= (style & WS_MINIMIZEBOX) || (style & WS_MAXIMIZEBOX)
        ? WS_SYSMENU : 0;
    // Make sure WS_CAPTION is on if WS_SYSMENU is.
    style |= (style & WS_SYSMENU) ? WS_CAPTION : 0;

    // Overlapped window. An overlapped window has a title bar and a border.
    //style |= WS_OVERLAPPED;
    // Pop-up window. This style cannot be used with the WS_CHILD style.
    //style |= WS_POPUP;

    // Pop-up window with WS_BORDER, WS_POPUP, and WS_SYSMENU styles. The
    // WS_CAPTION and WS_POPUPWINDOW styles must be combined to make the window
    // menu visible.
    //style |= WS_POPUPWINDOW 

    // Window that has a sizing border. Same as the WS_THICKFRAME style.
    style |= WS_SIZEBOX;
    // Window that is initially visible.
    style |= WS_VISIBLE;

    return style;
}

}} // namespace pt::win32
