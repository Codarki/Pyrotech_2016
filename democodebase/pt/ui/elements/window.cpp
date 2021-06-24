// TODO get rid of the disable
#pragma warning(push)
#pragma warning(disable: 4505)  // unreferenced local function has been removed

#include "window.h"
#include "../canvas.h"
#include "../close_event.h"
#include "../draw_event.h"
#include "../event_handler.h"
#include "../mouse_event.h"
#include "../native_window.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/debug_only.h"
#include "pt/log.h"
#include "pt/scoped_assignment.h"
#include "pt/win32/clipboard.h"
#include "pt/win32/font.h"
#include "pt/win32/font_utility.h"
#include "pt/win32/message_pump.h"
#include "pt/win32/utility.h"
#include "pt/win32/window.h"
#include "pt/win32/window_class.h"
#include "pt/win32/win32_error.h"
//#include "pt/d3d9/core.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/graphics_adapter.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/graphics_system.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"

namespace pt { namespace ui { namespace elements {

window::window(std::string const& name)
:   m_is_active(true)
,   m_has_kb_focus(true)
,   m_gamma(false)
,   m_sizing_or_moving(false)
,   m_larger_backbuffer_than_canvas(false)
{
    register_window_class(name);

    //LOGFONT font;
    //TEXTMETRIC metric;
    //LPARAM lparam;
    //int result FontEnumProc(const LOGFONT*, const TEXTMETRIC*, DWORD, LPARAM lParam);
}

window::~window()
{
    // If we are the parent of our content, null it out, becouse we dont
    // know if we are the sole owner.
    if (has_content())
    {
        if (content().parent() == this)
            content().set_parent(0);

		set_content(nullptr);
    }
    set_handler(nullptr);
}

void window::create(std::string const& name, int width, int height)
{
    if (m_native_window)
        throw any_error("duplicate window creation");

    DWORD style = win32::create_window_style(false, false, false);
    m_native_window.reset(new native_window(name, style, 0, 0, width, height,
        0, 0, m_window_class, this));

    m_client_size[0] = width;
    m_client_size[1] = height;

    //win32::choose_font(m_native_window->handle(), 0);
}

void window::create_with_client_size(std::string const& name, int width,
    int height)
{
    if (m_native_window)
        throw any_error("duplicate window creation");

    DWORD style = win32::create_window_style(false, false, false);

    // Add the size of borders to the window size, adjusting the client size
    // to be exactly [width, height]
    int x = 0;
    int y = 0;
    RECT client_rect;
    client_rect.left = x;
    client_rect.top = y;
    client_rect.right = x + width;
    client_rect.bottom = y + height;

    BOOL has_menu = 0;
    if (!AdjustWindowRect(&client_rect, style, has_menu))
        throw win32::win32_error("AdjustWindowRect");

    // Move if we adjusted to offscreen (negative top-left coordinate).
    if (client_rect.left < 0)
    {
        int x_offscreen = -client_rect.left;
        client_rect.left += x_offscreen;
        client_rect.right += x_offscreen;
    }

    if (client_rect.top < 0)
    {
        int y_offscreen = -client_rect.top;
        client_rect.top += y_offscreen;
        client_rect.bottom += y_offscreen;
    }

    m_client_size[0] = width; // client_rect.right - client_rect.left;
    m_client_size[1] = height; //client_rect.bottom - client_rect.top;

    m_native_window.reset(new native_window(name, style, client_rect, 0,
        0, m_window_class, this));

    //int sizing_border_size_x = GetSystemMetrics(SM_CXSIZEFRAME);

    //win32::enum_font_families(ANSI_CHARSET);
    //win32::choose_font(m_native_window->handle(), 0);

    /// \todo remove hard-coded font? or keep one default font?
    win32::log_font_container fonts = win32::enum_font(ANSI_CHARSET, "Tahoma");
    assert(!fonts.empty());
    HFONT hfont = CreateFontIndirect(&fonts[0]);
    hfont;
}

void window::init_graphics_system()
{
    // Create d3d graphics system.
    m_d3d.reset(new d3d9::graphics_system);
}

void window::init_direct3d(int msaa_count, bool fullscreen)
{
    m_larger_backbuffer_than_canvas = false;
    assert(m_native_window);
	if (msaa_count)
	{
	}

    d3d9::graphics_device_parameters parameters;
    //parameters.aa_count = msaa_count;
    //parameters.aa_quality = 0;
    parameters.bits = 32;
    parameters.fullscreen = fullscreen;
    parameters.refresh_rate = 0;
    parameters.width = m_client_size[0];
    parameters.height = m_client_size[1];

    if (fullscreen)
    {
    }
    else
    {
        RECT client_rect;
        if (!GetClientRect(m_native_window->handle(), &client_rect))
            throw win32::win32_error("GetClientRect");

        vector2i client_size(client_rect.right - client_rect.left,
            client_rect.bottom - client_rect.top);

        // TODO make sure sizes matches.. we dont have to do this.
        m_client_size = client_size;
    }

    if (!m_d3d)
        init_graphics_system();

    // We have larger backbuffer than canvas, when when we try to avoid
    // graphics device resets for every pixel on resizing. This will force
    // us to use copy swap effect and source rectangles in present.
    //
    // Do not rely on the size of backbuffer anywhere..
    // Use this only if resizing is allowed.

    if (m_larger_backbuffer_than_canvas)
    {
        parameters.swap = d3d9::swap_copy;
    }
    else
    {
        parameters.swap = d3d9::swap_discard;
    }

    m_d3d_device = graphics_adapter()->create_device(parameters,
        native()->handle());

    m_canvas.reset(new canvas(m_d3d_device));

    //d3d9::create_direct3d();
    //d3d9::create_device(client_size[0], client_size[1], 32, false,
    //    native()->handle());
}

void window::close_direct3d()
{
    m_canvas.reset();
    m_d3d_device.reset();
    m_d3d.reset();
}

std::shared_ptr<d3d9::graphics_adapter> window::graphics_adapter()
{
    assert(m_d3d);
    return m_d3d->adapter(D3DADAPTER_DEFAULT);
}

d3d9::graphics_device& window::direct3d_device()
{
    assert(m_d3d_device);
    return *m_d3d_device;
}

std::shared_ptr<native_window const> window::native() const
{
    return m_native_window;
}

void window::set_content(std::shared_ptr<bounded_element> const& content)
{
    m_content = content;
    if (m_content)
        m_content->set_bounds(box_from_min_max(vector2i(0, 0), m_client_size));
}

void window::set_handler(std::shared_ptr<event_handler> const& handler)
{
    m_handler = handler;
}

int window::run_message_pump()
{
    m_message_pump.reset(new win32::message_pump);
    return m_message_pump->run();
}

//void window::do_handle_event(event&)
//{
//}

void window::do_parse(config_node const&)
{
}

void window::do_invalidate()
{
    if (!InvalidateRect(native()->handle(), 0, false))
        throw win32::win32_error("InvalidateRect");
}

LRESULT window::handle_message(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
            return handle_mouse(msg, wparam, lparam);

        switch(msg)
        {
        case WM_ACTIVATE:
            if (HIWORD(wparam) != 0)
            {
                // is minimized
                //m_is_active = false;
            }
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                assert(m_is_active);
                m_is_active = false;
                //assert(!lparam);
                // lparam is the handle to the window being activated.
            }
            else if (LOWORD(wparam) == WA_ACTIVE
                || LOWORD(wparam) == WA_CLICKACTIVE)
            {
                //assert(!m_is_active);
                m_is_active = true;
                //assert(!lparam);
                // lparam is the handle to the window being deactivated
            }
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_ACTIVATEAPP:
            //bool activated = (wparam == TRUE);
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_SETFOCUS:
            assert(!m_has_kb_focus);
            m_has_kb_focus = true;
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);
            // sent to a window after it has gained the keyboard focus.
        case WM_KILLFOCUS:
            // sent to a window immediately before it loses the keyboard focus.
            // do not make any function calls that display or activate a window.
            assert(m_has_kb_focus);
            m_has_kb_focus = false;
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_ENTERSIZEMOVE:
            m_sizing_or_moving = true;
            return 0;
        case WM_EXITSIZEMOVE:
            {
                m_sizing_or_moving = false;

                // demo2009 hack
                return 0;

                //RECT rect;
                //PT_DEBUG_ONLY(BOOL result = )
                //    GetClientRect(m_native_window->handle(), &rect);
                //assert(result != 0);

                //do_resize(rect.right - rect.left, rect.bottom - rect.top);
            }
            //return 0;

        case WM_ERASEBKGND:
            return 0;

        case WM_CHAR:
            if (!m_has_kb_focus)
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
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

        case WM_MOUSEACTIVATE:
            log("WM_MOUSEACTIVATE");
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);

        case WM_PAINT:
            if (!m_is_active)
                return DefWindowProc(m_native_window->handle(), msg, wparam,
                    lparam);
            if (wparam)
                assert(false); //HDC hdc = reinterpret_cast<HDC>(msg.wparam);
            return handle_paint();

        case WM_SIZE:
            {
                // demo2009 hack
                return 0;

                //LRESULT result = handle_size(wparam, lparam);
                //if (result == 0)
                //    return result;
                //else
                //    return DefWindowProc(m_native_window->handle(), msg,
                //        wparam, lparam);
            }
        case WM_SIZING:
            return DefWindowProc(m_native_window->handle(), msg, wparam,
                lparam);
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
            return handle_dead_char(msg, wparam, lparam);

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

       // MessageBox(m_native_window->handle(), text, "Exception",
       //     MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);

        throw;
        //return DefWindowProc(m_native_window->handle(), msg, wparam,
        //        lparam);
    }
}

LRESULT window::handle_char(WPARAM wparam, LPARAM lparam)
{
    int repeat_count = static_cast<int>(lparam & 0xffff);
    bool is_extended = (lparam & (1 << 24)) != 0;
    is_extended;
    //bool is_alt_held_down = (lparam & (1 << 24)) != 0;
    //bool was_down_before = (lparam & (1 << 30)) != 0;
    //bool being_released = (lparam & (1 << 31)) != 0;

    //char arr = static_cast<char>(wparam);
    //log(format("%1%") % arr);

    m_context.set_current_tick(GetTickCount());

    character_event e(static_cast<char>(wparam), repeat_count);
    e.set_context(&m_context);

    handler().handle_event(e);

    if (!e.consumed() && e.character() == 'g')
        m_gamma = !m_gamma;

    return 0;
}

LRESULT window::handle_dead_char(UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(m_native_window->handle(), msg, wparam,
        lparam);
}

LRESULT window::handle_close()
{
    if (!has_content() && m_message_pump)
    {
        m_native_window->destroy();
        m_window_class.reset();

        if (m_native_window->instance_count() == 0)
            m_message_pump->quit();
        return 0;
    }

    m_context.set_current_tick(GetTickCount());

    close_event e;
    e.set_context(&m_context);

    handler().handle_event(e);

    if (e.ok())
    {
        m_native_window->destroy();
        m_window_class.reset();

        if (m_native_window->instance_count() == 0 && m_message_pump)
            m_message_pump->quit();
            //PostQuitMessage(0);
    }

    return 0;
}

LRESULT window::handle_key_down(WPARAM wparam, LPARAM lparam)
{
    int key_code = static_cast<int>(wparam);
    int repeat_count = static_cast<int>(lparam & 0xffff);
    bool is_extended = (lparam & (1 << 24)) != 0;
    bool was_down_before = (lparam & (1 << 30)) != 0;

    if (is_extended)
    {
        int debug= 0;
        debug;
    }

    win32::clipboard clipboard(m_native_window->handle());

    m_context.set_clipboard(&clipboard);
    m_context.set_current_tick(GetTickCount());

    key_down_event e(key_code, was_down_before, repeat_count);
    e.set_context(&m_context);

    handler().handle_event(e);

    return 0;
}

LRESULT window::handle_key_up(WPARAM, LPARAM)
{
    //int repeat_count = static_cast<int>(lparam & 0xffff);
    //bool is_extended = lparam & (1 << 24) != 0;
    //bool was_down_before = lparam & (1 << 30) != 0;

    return 0;
}

LRESULT window::handle_mouse(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!m_handler)
        return DefWindowProc(m_native_window->handle(), msg, wparam, lparam);

    m_context.set_current_tick(GetTickCount());

    vector2i position(LOWORD(lparam), HIWORD(lparam));

    /// \todo Recheck from MSDN that these are working correctly. Coded
    /// blindly without Win32 API.

    //SetFocus

    switch(msg)
    {
	case WM_MOUSEMOVE:
        {
            mouse_move_event e(bounds(), position);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            mouse_button_down_event e(bounds(), position, mouse_button_left);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_LBUTTONUP:
        {
            mouse_button_up_event e(bounds(), position, mouse_button_left);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_LBUTTONDBLCLK:
        {
            mouse_button_down_event e(bounds(), position, mouse_button_left);
            e.set_doubleclicked();
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_RBUTTONDOWN:
        {
            mouse_button_down_event e(bounds(), position, mouse_button_right);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_RBUTTONUP:
        {
            mouse_button_up_event e(bounds(), position, mouse_button_right);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_RBUTTONDBLCLK:
        {
            mouse_button_down_event e(bounds(), position, mouse_button_right);
            e.set_doubleclicked();
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_MBUTTONDOWN:
        {
            mouse_button_down_event e(bounds(), position, mouse_button_middle);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_MBUTTONUP:
        {
            mouse_button_up_event e(bounds(), position, mouse_button_middle);
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
    case WM_MBUTTONDBLCLK:
        {
            mouse_button_down_event e(bounds(), position, mouse_button_middle);
            e.set_doubleclicked();
            e.set_context(&m_context);
            handler().handle_event(e);
        }
        break;
//    case WM_MOUSEWHEEL:

    default:
        return DefWindowProc(m_native_window->handle(), msg, wparam,
            lparam);
    }
    return 0;
}

LRESULT window::handle_mouse_move(LPARAM )
{
    //int mouse_x = LOWORD(lparam);
    //int mouse_y = LOWORD(lparam);
    return 0;
}

LRESULT window::handle_paint()
{
    BOOL has_update_rect = GetUpdateRect(native()->handle(), 0, FALSE);
    if (!has_update_rect)
        return 0;

    RECT rect;
    if (!GetUpdateRect(native()->handle(), &rect, TRUE))
        throw win32::win32_error("GetUpdateRect");

    win32::scoped_paint paint(native()->handle());

    static bool is_drawing = false;

    if (is_drawing)
    {
        //assert(false);
        return 0;
    }

    if (!m_handler)
        return 0;

    //if (!d3d9::graphics_system::has_instance())
    //    return 0;
    //if (!d3d9::graphics_system::instance().adapter(
    //    D3DADAPTER_DEFAULT)->has_device())
    //    return 0;
    if (!m_canvas)
        return 0;

    scoped_assignment<bool> drawing(is_drawing, true);

    canvas::scoped_viewport scoped_viewport(*m_canvas, vector2i(0, 0),
        m_client_size);

    //draw_event e(win32::box_from(rect));
    m_context.set_current_tick(GetTickCount());
    draw_event e(*m_canvas);
    e.set_context(&m_context);

    {
        /// \todo remove this
        //d3d9::set_current_device(m_d3d_device);

        d3d9::scoped_scene scoped_scene(*m_d3d_device);
        //if (m_gamma)
        //    d3d9::scoped_render_state srgb_write(D3DRS_SRGBWRITEENABLE, TRUE);

        handler().handle_event(e);
    }
    //d3d9::set_current_device(shared_ptr<d3d9::graphics_device>());

    //com_ptr<IDirect3DSwapChain9> swap_chain;
    //d3d9::check_dx(m_d3d_device->native()->GetSwapChain(
    //    0, swap_chain.receive()),
    //    "IDirect3DDevice9::GetSwapChain");

    //DWORD flags = 0;
    //if (m_gamma)
    //    flags |= D3DPRESENT_LINEAR_CONTENT;

    //if (m_larger_backbuffer_than_canvas)
    //{
    //    RECT source_rect = win32::rect_from(vector2i(0, 0), m_client_size);

    //    d3d9::check_dx(swap_chain->Present(&source_rect, 0,
    //        native()->handle(), 0, flags), "IDirect3DSwapChain9::Present");
    //}
    //else
    //{
    //    d3d9::check_dx(swap_chain->Present(0, 0, native()->handle(), 0, flags),
    //        "IDirect3DSwapChain9::Present");
    //}

    d3d9::present(*m_d3d_device, 0, 0, native()->handle(), 0);

    return 0;
}

LRESULT window::handle_size(WPARAM wparam, LPARAM lparam)
{
    //if (m_sizing_or_moving)
    //    return -1;

    int new_width = LOWORD(lparam);
    int new_height = HIWORD(lparam);

    m_client_size[0] = new_width;
    m_client_size[1] = new_height;

    switch(wparam)
    {
    case SIZE_RESTORED:
        do_resize(new_width, new_height);
        return 0;

    case SIZE_MAXIMIZED:
        do_resize(new_width, new_height);
        return 0;

    case SIZE_MINIMIZED:
    case SIZE_MAXSHOW:
    case SIZE_MAXHIDE:
        return -1;
    }
    return -1;
}

bool window::has_content() const
{
    return m_content != nullptr;
}

bounded_element& window::content()
{
    if (!has_content())
        throw any_error("ui::window with empty content");
    return *m_content;
}

event_handler& window::handler()
{
    if (!m_handler)
        throw any_error("ui::window without event handler");
    return *m_handler;
}

void window::do_resize(int width, int height)
{
    m_client_size[0] = width;
    m_client_size[1] = height;

    if (has_content())
    {
        content().set_bounds(box_from_min_max(vector2i(0, 0),
            m_client_size));
    }

    if (m_d3d_device)
    {
        if (m_larger_backbuffer_than_canvas)
        {
            int new_width = m_d3d_device->parameters().BackBufferWidth;
            int new_height = m_d3d_device->parameters().BackBufferHeight;

            int const width_difference = new_width - width;
            int const height_difference = new_height - height;

            int const margin = 128;

            if (width_difference < 0 || width_difference > margin * 2
                || height_difference < 0 || height_difference > margin * 2)
            {
                // Adjust size with steps of margin.
                if (width_difference < 0 || width_difference > margin * 2)
                    new_width = (width + margin) / margin * margin;

                if (height_difference < 0 || height_difference > margin * 2)
                    new_height = (height + margin) / margin * margin;

                m_d3d_device->reset(new_width, new_height);
            }
        }
        else
        {
            m_d3d_device->reset(width, height);
        }
    }
}

void window::register_window_class(std::string const& name)
{
    if (m_window_class)
        throw any_error("duplicate window class registration");

    // If this parameter is 0, GetModuleHandle returns a handle to the file
    // used to create the calling process (.exe file).
    HMODULE module_handle = GetModuleHandle(0);

    HICON icon;
   // HINSTANCE hExe = LoadLibrary("project_avarus.exe");
    //if (hExe == NULL)
    //{
        //Error loading module -- fail as securely as possible
        icon = LoadIcon(NULL, IDI_APPLICATION);
    //}
    //else
    //{
    //    icon = LoadIcon(hExe, "pyrotech.ico");
    //}
    

    //HICON icon = LoadIcon(NULL, IDI_APPLICATION);
    HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);

    m_window_class.reset(new win32::window_class(name, "", module_handle,
        native_window::wnd_proc, icon, cursor));
}

}}} // namespace pt::ui::elements

//void gammacorrect(double gamma)
//{ 
//    int i; 
//    unsigned char v, nv; 
//    double dv; 
// 
//    for (int i = 0; i < 8; ++i)
//    {
//        v = threeto8[i]; 
//        dv = (255.0 * pow(v/255.0, 1.0/gamma)) + 0.5;
//        nv = (unsigned char)dv; 
//        printf("Gamma correct %d to %d (gamma %.2f)\n", v, nv, gamma);
//        threeto8[i] = nv;
//    }
//
//    for (int i = 0; i < 4; ++i)
//    {
//        v = twoto8[i]; 
//        dv = (255.0 * pow(v/255.0, 1.0/gamma)) + 0.5; 
//        nv = (unsigned char)dv; 
//        printf("Gamma correct %d to %d (gamma %.2f)\n", v, nv, gamma); 
//        twoto8[i] = nv; 
//    } 
//    printf("\n"); 
//} 

//#pragma warning(pop)
