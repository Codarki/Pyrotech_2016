#ifndef PT_UI_MOUSE_MOVE_EVENT_HANDLER_H
#define PT_UI_MOUSE_MOVE_EVENT_HANDLER_H

namespace pt { namespace ui {

class mouse_move_event;

/// Handler for mouse movement events.
class mouse_move_event_handler
{
public:
    /// Handles a mouse movement event. Default implementation does nothing.
    virtual void handle_mouse_move(mouse_move_event& event);
};

}} // namespace pt::ui

#endif
