#ifndef PT_UI_CHARACTER_EVENT_HANDLER_H
#define PT_UI_CHARACTER_EVENT_HANDLER_H

namespace pt { namespace ui {

class character_event;

/// Handler for character events.
class character_event_handler
{
public:
    /// Handles a character event. Default implementation does nothing.
    virtual void handle_character(character_event& event);
};

}} // namespace pt::ui

#endif
