#ifndef PT_UI_KEY_DOWN_EVENT_HANDLER_H
#define PT_UI_KEY_DOWN_EVENT_HANDLER_H

namespace pt { namespace ui {

class key_down_event;

/// Handler for key down events.
class key_down_event_handler
{
public:
    /// Handles a key down event. Default implementation does nothing.
    virtual void handle_key_down(key_down_event& event);
};

}} // namespace pt::ui

#endif
