#ifndef PT_UI_EVENT_HANDLER_H
#define PT_UI_EVENT_HANDLER_H

#include "generic_event_handler.h"
#include "close_event_handler.h"
#include "draw_event_handler.h"
#include "key_down_event_handler.h"
#include "character_event_handler.h"
#include "mouse_button_event_handler.h"
#include "mouse_move_event_handler.h"

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
/// implement. Only subclasses can dispatch an event. Correct handling
/// function is called automatically by event when it is dispatched by this
/// handler. Default event handling implementations does nothing.
///
/// Subclass can decide wheter they propagate the event to child handlers
/// or handle the event itself first.
///
/// There is only one public function in this class, which is the virtual
/// abstract handler for \link ui::event events\endlink.
class event_handler : public generic_event_handler,
    protected close_event_handler,
    protected draw_event_handler,
    protected key_down_event_handler,
    protected character_event_handler,
    protected mouse_button_event_handler,
    protected mouse_move_event_handler
{
public:
    virtual ~event_handler();

    // Refer to doxygen documentation to easily see inherited interface.

protected:
    /// Dispatches an \a event to this event handler. Only subclasses are
    /// allowed to dispatch an event. Dispatching an event avoids the use of
    /// dynamic casts to find out the type of an event.
    void dispatch(event& event);

private:
    friend event;
    friend character_event;
    friend close_event;
    friend draw_event;
    friend key_down_event;
    friend mouse_button_down_event;
    friend mouse_button_up_event;
    friend mouse_move_event;
};

}} // namespace pt::ui

#endif
