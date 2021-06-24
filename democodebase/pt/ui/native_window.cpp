#include "native_window.h"
#include "elements/window.h"
#include "pt/win32/window.h"
#include "pt/win32/win32_error.h"
#include "pt/any_error.h"
#include "pt/debug_only.h"
#include "pt/log.h"
#include "pt/format.h"

namespace pt { namespace ui {

native_window::map native_window::m_instances;

native_window::native_window(std::string const& name, DWORD style, int x,
    int y, int width, int height, win32::window* parent, HMENU menu,
    std::shared_ptr<win32::window_class> const& window_class,
    ui::elements::window* window)
:   m_ui_window(window)
{
    assert(m_ui_window);

    // Keep in mind, this call gets directed to wnd_proc with WM_CREATE before
    // it returns.
    m_window.reset(new win32::window(name, style, x, y, width, height, parent,
        menu, window_class));

    register_instance(m_window->handle(), this);
}
native_window::native_window(std::string const& name, DWORD style,
    RECT const& rect, win32::window* parent, HMENU menu,
    std::shared_ptr<win32::window_class> const& window_class,
    ui::elements::window* window)
:   m_ui_window(window)
{
    assert(m_ui_window);

    // Keep in mind, this call gets directed to wnd_proc with WM_CREATE before
    // it returns.
    m_window.reset(new win32::window(name, style, rect, parent, menu,
        window_class));

    register_instance(m_window->handle(), this);
}


native_window::~native_window()
{
}

LRESULT CALLBACK native_window::wnd_proc(HWND handle, UINT msg, WPARAM wparam,
    LPARAM lparam)
{
    // TODO Exception handling. We CAN NOT let them fall through here.

    native_window* instance = find_instance(handle);

    static bool disable_message_handling = false;

    if (instance && !disable_message_handling)
    {
        try
        {
            LRESULT result = instance->m_ui_window->handle_message(msg, wparam,
                lparam);
            return result;
        }
        catch(std::exception& e)
        {
            disable_message_handling = true;
            log(format("preventing exception from leaving window procedure: %1%")
                % e.what());
        }
        catch(...)
        {
            disable_message_handling = true;
            log("preventing exception from leaving window procedure...");
        }
        PostQuitMessage(-1);
        return 0;
    }
    else
    {
        return DefWindowProc(handle, msg, wparam, lparam);
    }
}

HWND native_window::handle() const
{
    if (!m_window)
        throw any_error("Getting null window handle");
    return m_window->handle();
}

void native_window::destroy()
{
    if (!m_window)
        throw any_error("Destroying invalid window");

    unregister_instance(m_window->handle(), this);

    // Issues WM_DESTROY before returning.
    m_window.reset();
}

size_t native_window::instance_count()
{
    return m_instances.size();
}

/// Returns native window instance corresponding to \a handle from static map.
native_window* native_window::find_instance(HWND handle)
{
    map::const_iterator i = m_instances.find(handle);
    if (i != m_instances.end())
        return i->second;

    return 0;
}

/// Registers \a instance with \a handle into static map.
void native_window::register_instance(HWND handle, native_window* instance)
{
    assert(handle && instance);

    if (native_window* p = find_instance(handle))
        throw any_error("Duplicate window instance registration.");

    m_instances[handle] = instance;
}

/// Unregisters \a instance with \a handle in static map.
void native_window::unregister_instance(HWND handle,
    native_window* PT_DEBUG_ONLY(instance))
{
    assert(handle && instance);

    map::iterator i = m_instances.find(handle);
    if (i != m_instances.end())
    {
        assert(i->second == instance);
        m_instances.erase(i);
        return;
    }

    throw any_error("Invalid window instance unregistration.");
}

}} // namespace pt::ui
