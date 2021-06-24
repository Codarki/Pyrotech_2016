#ifndef PT_UI_DRAW_EVENT_HANDLER_H
#define PT_UI_DRAW_EVENT_HANDLER_H

namespace pt { namespace ui {

class draw_event;

/// Handler for draw events.
class draw_event_handler
{
public:
    /// Handles a draw event. Default implementation does nothing.
    virtual void handle_draw(draw_event& event);
};

}} // namespace pt::ui

#endif
