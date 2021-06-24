#ifndef PT_UI_ELEMENTS_WINDOW_H
#define PT_UI_ELEMENTS_WINDOW_H

#include "pt/ui/bounded_element.h"
#include "pt/ui/event_context.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/win32.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {
    class graphics_adapter;
    class graphics_system;
}} // namespace pt::dx

namespace pt { namespace win32 {
    class message_pump;
    class window_class;
}} // namespace pt::win32

namespace pt { namespace ui {
    class canvas;
    class event_context;
    class event_handler;
    class native_window;
}} // namespace pt::ui

namespace pt { namespace ui { namespace elements {

/// UI window element.
//class window : public value_element, public bounded_element
class window : public bounded_element
{
public:
    /// Registers win32::window_class.
    window(std::string const& name);
    ~window();

    /// Creates this window with actual size of \a width and \a height.
    /// \note This size includes the borders, so the client size is smaller
    /// than given size.
    void create(std::string const& name, int width, int height);

    /// Creates this window with client size of \a width and \a height.
    /// \note The actual window size includes border, and is therefore larger
    /// than given size.
    void create_with_client_size(std::string const& name, int width,
        int height);

    /// \todo there can be only one "direct3d device owner window"
    void init_graphics_system();
    void init_direct3d(int msaa_count, bool fullscreen);
	std::shared_ptr<d3d9::graphics_adapter> graphics_adapter();
    d3d9::graphics_device& direct3d_device();
    void close_direct3d();

	std::shared_ptr<native_window const> native() const;

    /// Sets the \a content for this window.
    void set_content(std::shared_ptr<bounded_element> const& content);

    /// Sets the event \a handler for this window. Usually this is the same
    /// as the content.
    void set_handler(std::shared_ptr<event_handler> const& handler);

    /// \todo temporary hack to use message_pump and make close events work.
    /// Should be elsewhere.
    int run_message_pump();

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

private: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // interface for native_window
    friend native_window;
    LRESULT handle_message(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT handle_char(WPARAM wparam, LPARAM lparam);
    LRESULT handle_dead_char(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT handle_close();
    LRESULT handle_key_down(WPARAM wparam, LPARAM lparam);
    LRESULT handle_key_up(WPARAM wparam, LPARAM lparam);
    LRESULT handle_mouse(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT handle_mouse_move(LPARAM lparam);
    LRESULT handle_paint();
    LRESULT handle_size(WPARAM wparam, LPARAM lparam);

private: // helper functions
    bool has_content() const;
    bounded_element& content();
    event_handler& handler();
    void do_resize(int width, int height);
    void register_window_class(std::string const& name);

private: // data members
    std::shared_ptr<bounded_element> m_content;
    std::shared_ptr<event_handler> m_handler;
    std::shared_ptr<native_window> m_native_window;
    std::shared_ptr<win32::window_class> m_window_class;
    std::shared_ptr<d3d9::graphics_system> m_d3d;
	std::shared_ptr<d3d9::graphics_device> m_d3d_device;
    std::shared_ptr<win32::message_pump> m_message_pump;
    vector2i m_client_size;
    std::unique_ptr<ui::canvas> m_canvas;

    event_context m_context;

    bool m_is_active;
    bool m_has_kb_focus;
    bool m_gamma;
    bool m_sizing_or_moving;
    bool m_larger_backbuffer_than_canvas;
};

}}} // namespace pt::ui::elements

#endif
