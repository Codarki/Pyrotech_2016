#ifndef PT_UI_GENERIC_EVENT_HANDLER_H
#define PT_UI_GENERIC_EVENT_HANDLER_H

namespace pt { namespace ui {

class event;

/// Abstract base class for event handlers.
class generic_event_handler
{
public:
    /// Handles untyped event.
    virtual void handle_event(event& event) = 0;
};

}} // namespace pt::ui

#endif
