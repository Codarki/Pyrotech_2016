#include "event.h"
#include "event_context.h"
#include "pt/win32.h"
#include "pt/std/cassert.h"

namespace pt { namespace ui {

// event_context

event_context::event_context()
:   m_tick(0)
,   m_clipboard(0)
,   m_invalidate_requested(false)
{
}

void event_context::set_current_tick(uint32 tick)
{
    m_tick = tick;
}

uint32 event_context::current_tick() const
{
    return m_tick;
}

void event_context::set_clipboard(win32::clipboard const* clipboard)
{
    m_clipboard = clipboard;
}

win32::clipboard const& event_context::clipboard() const
{
    assert(m_clipboard);
    return *m_clipboard;
}

void event_context::invalidate()
{
    m_invalidate_requested = true;
}

void event_context::clear_invalidate_request()
{
    m_invalidate_requested = false;
}

// event
event::event()
:   m_context(0)
{
}

event::~event()
{
}

void event::set_context(event_context* context)
{
    m_context = context;
}

event_context const& event::get_context() const
{
    assert(m_context);
    return *m_context;
}

void event::dispatch(event_handler& handler)
{
    do_dispatch(handler);
}

}} // namespace pt::ui
