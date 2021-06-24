#ifndef PT_UI_MOUSE_BUTTON_EVENT_HANDLER_H
#define PT_UI_MOUSE_BUTTON_EVENT_HANDLER_H

namespace pt { namespace ui {

class mouse_button_event;
class mouse_button_down_event;
class mouse_button_up_event;

/// Handler for mouse button down events.
class mouse_button_down_event_handler
{
public:
    /// Handles a mouse button down event. Default implementation does
    /// nothing.
    virtual void handle_mouse_button_down(mouse_button_down_event& event);
};

/// Handler for mouse button up events.
class mouse_button_up_event_handler
{
public:
    /// Handles a mouse button up event. Default implementation does
    /// nothing.
    virtual void handle_mouse_button_up(mouse_button_up_event& event);
};

/// Handler for mouse button events.
class mouse_button_event_handler
    : public mouse_button_down_event_handler,
    public mouse_button_up_event_handler
{
public:
    /// Handles a mouse button event. Default implementation does nothing.
    virtual void handle_mouse_button(mouse_button_event& event);
};

}} // namespace pt::ui

#endif
