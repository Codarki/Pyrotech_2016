#ifndef PT_UI_WINDOW_H
#define PT_UI_WINDOW_H

#include "element.h"
#include "pt/vector.h"
#include "pt/win32.h"
#include "pt/boost/noncopyable.h"
#include "pt/std/string.h"

namespace pt { namespace dx {
    class graphics_system;
}} // namespace pt::dx

namespace pt { namespace win32 {
    class window_class;
}} // namespace pt::win32

namespace pt { namespace ui {

class native_window;

/// User interface window.
///
/// This class functions as a proxy between \link win32::window win32
/// windows \endlink and associated content, an ui::element. This class
/// receives windows messages, creates UI events and forwards them to UI
/// elements.
///
/// \note There can be multiple ui::window classes in same process (and
/// thread.)
///
/*
class window : public element, private noncopyable
{
public:
    /// Creates unregistered and nonexistant window.
    window();
    /// Registers win32::window_class.
    window(std::string const& name);

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
    void init_direct3d(bool fullscreen);

    shared_ptr<native_window const> native() const;

    /// Sets the \a content for this window.
    void set_content(shared_ptr<element> const& content);

public: // interface required by element
    void handle_event(shared_ptr<event> const& event);
    void parse(config_node const& config);

public: // interface required by invalidatable
    void invalidate();

private: // type definitions
private: // helper functions
    shared_ptr<element> const& content();
    void register_window_class(std::string const& name);

private: // interface for native_window
    friend native_window;
    LRESULT handle_message(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT handle_char(WPARAM wparam, LPARAM lparam);
    LRESULT handle_close();
    LRESULT handle_key_down(WPARAM wparam, LPARAM lparam);
    LRESULT handle_key_up(WPARAM wparam, LPARAM lparam);
    LRESULT handle_paint();
    LRESULT handle_size(WPARAM wparam, LPARAM lparam);

private: // data members
    shared_ptr<element> m_content;
    shared_ptr<native_window> m_native_window;
    shared_ptr<win32::window_class> m_window_class;
    shared_ptr<dx::graphics_system> m_d3d;
    vector2i m_client_size;
};
*/

}} // namespace pt::ui

#endif
