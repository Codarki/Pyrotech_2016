#ifndef PT_UI_NATIVE_WINDOW_H
#define PT_UI_NATIVE_WINDOW_H

#include "pt/boost/noncopyable.h"
#include "pt/win32.h"
#include "pt/std/map.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace win32 {
    class window;
    class window_class;
}}

namespace pt { namespace ui {

namespace elements {
    class window;
} // namespace elements

/// Utility class for static container of mapping pt::win32::window instances
/// to correct pt::ui::window instances.
class native_window : boost::noncopyable
{
public:
    /// Constructs new window instance.
    native_window(std::string const& name, DWORD style, int x, int y,
        int width, int height, win32::window* parent, HMENU menu,
		std::shared_ptr<win32::window_class> const& window_class,
        ui::elements::window* window);
    native_window(std::string const& name, DWORD style, RECT const& rect,
        win32::window* parent, HMENU menu,
        std::shared_ptr<win32::window_class> const& window_class,
        ui::elements::window* window);

    ~native_window();

    /// Single win32 window procedure for this process (exe or dll) that is
    /// registered in pt::win32::window_class. Forwards all messages to
    /// correct instance of associated ui::window.
    static LRESULT CALLBACK wnd_proc(HWND handle, UINT msg, WPARAM wp,
        LPARAM lp);

    /// \return Native window handle for this window.
    HWND handle() const;

    /// Destroys the owned win32::window and unregisters this window instance
    /// from receiving events from window procedure.
    void destroy();

    static size_t instance_count();

private: // type definitions
    typedef std::map<HWND,native_window*> map;

private: // helper functions
    static native_window* find_instance(HWND handle);
    static void register_instance(HWND handle, native_window* instance);
    static void unregister_instance(HWND handle, native_window* instance);

private: // data members
    ui::elements::window* m_ui_window;
    std::shared_ptr<win32::window> m_window;
    static map m_instances;
};

}} // namespace pt::ui

#endif
