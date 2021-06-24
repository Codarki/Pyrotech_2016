#ifndef PT_UI_EVENT_CONTEXT_H
#define PT_UI_EVENT_CONTEXT_H

#include "pt/basic_types.h"

namespace pt { namespace win32 {
    class clipboard;
}}

namespace pt { namespace ui {

// predeclarations.
//class event_handler;
//class close_event;
//class draw_event;
//class input_event;
//class keyboard_event;
//class key_down_event;
//class character_event;
//class mouse_event;
//class mouse_button_event;
//class mouse_button_down_event;
//class mouse_button_up_event;
//class mouse_move_event;

class event_context
{
public:
    event_context();

    void set_current_tick(uint32 tick);
    uint32 current_tick() const;

    void set_clipboard(win32::clipboard const* clipboard);
    win32::clipboard const& clipboard() const;

    void invalidate();
    void clear_invalidate_request();

private: // data members
    uint32 m_tick;
    win32::clipboard const* m_clipboard;
    bool m_invalidate_requested;
};

}}

#endif
