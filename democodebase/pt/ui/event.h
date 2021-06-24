#ifndef PT_UI_EVENT_H
#define PT_UI_EVENT_H

#include "pt/basic_types.h"

namespace pt { namespace ui {

// predeclarations.
class event_context;
class event_handler;
class close_event;
class draw_event;
class input_event;
class keyboard_event;
class key_down_event;
class character_event;
class mouse_event;
class mouse_button_event;
class mouse_button_down_event;
class mouse_button_up_event;
class mouse_move_event;

/// Abstract base class for all UI events.
class event
{
public: // type definitions
    

public: // interfafce subclasses must implement
    event();
    virtual ~event();

    void set_context(event_context* context);
    event_context const& get_context() const;

private:
    /// Dispatches this event to a \a handler. Subclasses are required to
    /// call correct handling function. This avoid the use of
    /// dynamic_cast(). Only event_handler can call this.
    friend event_handler;
    void dispatch(event_handler& handler);

private: // virtual functions which subclasses must implement
    virtual void do_dispatch(event_handler& handler) = 0;

private: // data members
    event_context* m_context;
};

}} // namespace pt::ui

#endif
