#include "window.h"
#include "element.h"
#include "element_registry.h"
#include "event.h"
#include "native_window.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/log.h"
#include "pt/scoped_assignment.h"
#include "pt/win32/utility.h"
#include "pt/win32/window_class.h"
#include "pt/win32/win32_error.h"
#include "pt_dx/core.h"
#include "pt_dx/graphics_adapter.h"
#include "pt_dx/graphics_system.h"

namespace pt { namespace ui {
/*
window::window()
{
}

window::window(std::string const& name)
{
    register_window_class(name);
}

void window::create(std::string const& name, int width, int height)
{
    if(m_native_window)
        throw any_error("duplicate window creation");

    DWORD style = win32::create_window_style(false, false, false);
    m_native_window.reset(new native_window(name, style, 0, 0, width, height,
        0, 0, m_window_class, this));

    m_client_size[0] = width;
    m_client_size[1] = height;
}

void window::create_with_client_size(std::string const& name, int width,
    int height)
{
    if(m_native_window)
        throw any_error("duplicate window creation");

    DWORD style = win32::create_window_style(false, false, false);

    // Add the size of borders to the window size
    int x = 0;
    int y = 0;
    RECT client_rect;
    client_rect.left = x;
    client_rect.top = y;
    client_rect.right = x + width;
    client_rect.bottom = y + height;

    BOOL has_menu = 0;
    if(!AdjustWindowRect(&client_rect, style, has_menu))
        throw win32::win32_error("AdjustWindowRect");

    // Move if we adjusted to offscreen.
    if(client_rect.left < 0)
    {
        int x_offscreen = -client_rect.left;
        client_rect.left += x_offscreen;
        client_rect.right += x_offscreen;
    }

    if(client_rect.top < 0)
    {
        int y_offscreen = -client_rect.top;
        client_rect.top += y_offscreen;
        client_rect.bottom += y_offscreen;
    }

    m_client_size[0] = client_rect.right - client_rect.left;
    m_client_size[1] = client_rect.bottom - client_rect.top;

    m_native_window.reset(new native_window(name, style, client_rect, 0,
        0, m_window_class, this));

    //int sizing_border_size_x = GetSystemMetrics(SM_CXSIZEFRAME);
}

void window::init_direct3d(bool fullscreen)
{
    assert(m_native_window);

    if(fullscreen)
    {
    }
    else
    {
        RECT client_rect;
        if(!GetClientRect(m_native_window->handle(), &client_rect))
            throw win32::win32_error("GetClientRect");

        vector2i client_size(client_rect.right - client_rect.left,
            client_rect.bottom - client_rect.top);

        // TODO make sure sizes matches.. we dont have to do this.
        m_client_size = client_size;
    }

    // create graphics

    m_d3d.reset(new dx::graphics_system);
    m_d3d->create_device(D3DADAPTER_DEFAULT, m_client_size[0], m_client_size[1],
        32, native()->handle(), fullscreen);

    //dx::create_direct3d();
    //dx::create_device(client_size[0], client_size[1], 32, false,
    //    native()->handle());
}

shared_ptr<native_window const> window::native() const
{
    return m_native_window;
}

void window::set_content(shared_ptr<element> const& content)
{
    m_content = content;
}

void window::handle_event(shared_ptr<event> const&)
{
    // TODO This shouldn't be called unless this is a child window. In that
    // case implement this.
    assert(false);
}

void window::parse(config_node const& config)
{
    std::string window_class_name;
    std::string window_name;
    int width;
    int height;
    bool size_includes_borders;

    assert(config.key() == "window");

    config.parse_child("name", window_name);
    config.parse_child("width", width);
    config.parse_child("height", height);

    config.parse_optional_child("class_name", window_class_name,
        window_name);
    config.parse_optional_child("size_includes_borders", size_includes_borders,
        false);

    register_window_class(window_class_name);
    if(size_includes_borders)
        create(window_name, width, height);
    else
        create_with_client_size(window_name, width, height);

    config_node::const_iterator i = config.child("content");

    for(config_node::const_iterator i = config.begin(); i != config.end(); ++i)
    {
        m_content = element_registry::instance().create_optional(i->key());

        if(m_content)
        {
            m_content->parse(*i);
            break;
        }
    }
    if(!m_content)
        throw any_error("could not parse content for ui::window");
}

void window::invalidate()
{
    if(!InvalidateRect(native()->handle(), 0, false))
        throw win32::win32_error("InvalidateRect");
}

shared_ptr<element> const& window::content()
{
    if(!m_content)
        throw any_error("ui::window with empty content");
    return m_content;
}

void window::register_window_class(std::string const& name)
{
    if(m_window_class)
        throw any_error("duplicate window class registration");

    // If this parameter is 0, GetModuleHandle returns a handle to the file
    // used to create the calling process (.exe file).
    HMODULE module_handle = GetModuleHandle(0);

    m_window_class.reset(new win32::window_class(name, "", module_handle,
        native_window::wnd_proc));
}

LRESULT window::handle_message(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch(msg)
        {
        case WM_ACTIVATE:
            //if(LOWORD(wparam) == WA_INACTIVE)
                // lparam is the handle to the window being activated.
            //else if(LOWORD(wparam) == WA_ACTIVE || LOWORD(wparam) == WA_CLICKACTIVE)
                // lparam is the handle to the window being deactivated
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_ACTIVATEAPP:
            //bool activated = (wparam == TRUE);
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_SETFOCUS:
            // sent to a window after it has gained the keyboard focus.
        case WM_KILLFOCUS:
            // sent to a window immediately before it loses the keyboard focus.
            // do not make any function calls that display or activate a window.
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_ERASEBKGND:
            return 0;

        case WM_CHAR:
            return handle_char(wparam, lparam);

        case WM_CLOSE:
            return handle_close();

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            return handle_key_down(wparam, lparam);

        case WM_KEYUP:
            return handle_key_up(wparam, lparam);

        case WM_PAINT:
            if(wparam)
                assert(false); //HDC hdc = reinterpret_cast<HDC>(msg.wparam);
            return handle_paint();

        case WM_SIZE:
            {
                LRESULT result = handle_size(wparam, lparam);
                if(result == 0)
                    return result;
                else
                    return DefWindowProc(m_native_window->handle(), msg,
                        wparam, lparam);
            }
        case WM_SYSCOMMAND:
            switch(wparam)
            {
            case SC_CLOSE:
                //closes the window
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_MAXIMIZE:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_MINIMIZE:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_MONITORPOWER:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_MOVE:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_RESTORE:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_SCREENSAVE:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            case SC_SIZE:
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            }
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_DEADCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_SYSCHAR:
        case WM_SYSDEADCHAR:
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_DISPLAYCHANGE:
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        default:
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);
        }
    }
    catch(std::exception& e)
    {
        // TODO MB_ABORTRETRYIGNORE
        char const* text = e.what();
        log(text);
        MessageBox(m_native_window->handle(), text, "Exception",
            MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
        return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);
    }
}

LRESULT window::handle_char(WPARAM wparam, LPARAM lparam)
{
    int repeat_count = static_cast<int>(lparam & 0xffff);
    //bool is_extended = (lparam & (1 << 24)) != 0;
    //bool is_alt_held_down = (lparam & (1 << 24)) != 0;
    //bool was_down_before = (lparam & (1 << 30)) != 0;
    //bool being_released = (lparam & (1 << 31)) != 0;

    character_event e(static_cast<char>(wparam), repeat_count);
    e.set_time_s(GetTickCount() * 0.001f - content()->start_time());

    content()->handle_event(shared_ptr<event>(&e, null_deleter));

    return 0;
}

LRESULT window::handle_close()
{
    close_event e;
    e.set_time_s(GetTickCount() * 0.001f - content()->start_time());

    content()->handle_event(shared_ptr<event>(&e, null_deleter));
    return 0;
}

LRESULT window::handle_key_down(WPARAM wparam, LPARAM lparam)
{
    int key_code = static_cast<int>(wparam);
    int repeat_count = static_cast<int>(lparam & 0xffff);
    //bool is_extended = (lparam & (1 << 24)) != 0;
    bool was_down_before = (lparam & (1 << 30)) != 0;

    key_down_event e(key_code, was_down_before, repeat_count);
    e.set_time_s(GetTickCount() * 0.001f - content()->start_time());

    content()->handle_event(shared_ptr<event>(&e, null_deleter));

    return 0;
}

LRESULT window::handle_key_up(WPARAM, LPARAM)
{
    //int repeat_count = static_cast<int>(lparam & 0xffff);
    //bool is_extended = lparam & (1 << 24) != 0;
    //bool was_down_before = lparam & (1 << 30) != 0;

    return 0;
}

LRESULT window::handle_paint()
{
    BOOL has_update_rect = GetUpdateRect(native()->handle(), 0, FALSE);
    if(!has_update_rect)
        return 0;

    RECT rect;
    if(!GetUpdateRect(native()->handle(), &rect, TRUE))
        throw win32::win32_error("GetUpdateRect");

    win32::scoped_paint paint(native()->handle());

    static bool is_drawing = false;

    if(is_drawing)
    {
        //assert(false);
        return 0;
    }

    if(m_d3d->adapter(D3DADAPTER_DEFAULT)->has_device() && m_content)
    {
        scoped_assignment<bool> drawing(is_drawing, true);
        draw_event e;
        e.set_time_s(GetTickCount() * 0.001f - content()->start_time());

        content()->handle_event(shared_ptr<event>(&e, null_deleter));

        dx::present(0, 0, 0, 0);
        is_drawing = false;
    }

    return 0;
}

LRESULT window::handle_size(WPARAM wparam, LPARAM)
{
    //int new_width = LOWORD(lparam);
    //int new_height = HIWORD(lparam);

    switch(wparam)
    {
    case SIZE_RESTORED:
    case SIZE_MINIMIZED:
    case SIZE_MAXIMIZED:
    case SIZE_MAXSHOW:
    case SIZE_MAXHIDE:
        return -1;
    }
    return -1;
}
*/

}} // namespace pt::ui
