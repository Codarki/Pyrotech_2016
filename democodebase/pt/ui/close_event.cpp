#include "close_event.h"
#include "event_handler.h"

namespace pt { namespace ui {

close_event::close_event()
:   m_ok(false)
{
}

bool close_event::ok()
{
    return m_ok;
}

void close_event::set_ok_cancel(bool ok)
{
    m_ok = ok;
}

void close_event::do_dispatch(event_handler& handler)
{
    handler.handle_close(*this);
}

}} // namespace pt::ui
