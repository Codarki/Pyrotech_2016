#ifndef PT_UI_EVENT_HANDLER_H
#define PT_UI_EVENT_HANDLER_H

namespace pt { namespace ui {

// predeclarations.
class event;
class character_event;
class close_event;
class draw_event;
class key_down_event;
class mouse_button_down_event;
class mouse_button_up_event;
class mouse_move_event;

/// Abstract base class for UI event handlers.
///
/// Subclasses are free to choose which part of the interface they
/// implement. Correct handling function is called automatically by event
/// when it is dispatched to this handler. Default event handling
/// implementations does nothing.
///
/// Event handling has two functionalities.
/// \li Generic event handling which can dispatch and propagate event to
///     child handlers, without knowing which event handling they implement.
/// \li Specialized event handling, which can propagate to child handler
///     only the type of event it implements.
///
/// Class function members are self explanatory and thus undocumented.
///
/// \todo Maybe move the consumed flag to the event class, so we can
/// avoid calling this if event has been handled by specialized function.
class event_handler
{
public:
    virtual ~event_handler();

    /// Handles an untyped \a event. \note This function is never called
    /// from event dispatching, since this can be called directly.
    virtual void handle_event(event& event) = 0;

    virtual void handle_character(character_event& event);
    virtual void handle_close(close_event& event);
    virtual void handle_draw(draw_event& event);
    virtual void handle_key_down(key_down_event& event);
    virtual void handle_mouse_button_down(mouse_button_down_event& event);
    virtual void handle_mouse_button_up(mouse_button_up_event& event);
    virtual void handle_mouse_move(mouse_move_event& event);
};

}} // namespace pt::ui

#endif
