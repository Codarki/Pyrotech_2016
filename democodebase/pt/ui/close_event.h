#ifndef PT_UI_CLOSE_EVENT_H
#define PT_UI_CLOSE_EVENT_H

#include "event.h"

namespace pt { namespace ui {

class close_event_handler;

/// Close event. Typically issued when user is trying to close an element or
/// the application. This event gives the decision to event_handler.
class close_event : public event
{
public:
    /// Constructs a new close event with default value of \c false.
    close_event();

    /// \return \c true if this event was handled and close request
    /// accepted.
    bool ok();
    /// Accepts or denies the close request.
    void set_ok_cancel(bool ok);

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);

private: // data members
    bool m_ok;
};

}} // namespace pt::ui

#endif
