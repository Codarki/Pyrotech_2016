#ifndef PT_UI_CLOSE_EVENT_HANDLER_H
#define PT_UI_CLOSE_EVENT_HANDLER_H

namespace pt { namespace ui {

class close_event;

/// Handler for close events.
class close_event_handler
{
public:
    /// Handles a close event. Default implementation does nothing.
    virtual void handle_close(close_event& event);
};

}} // namespace pt::ui

#endif
