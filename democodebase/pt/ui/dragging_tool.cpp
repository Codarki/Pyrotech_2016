#include "dragging_tool.h"
#include "event.h"
#include "event_context.h"
#include "pt/log.h"

namespace pt { namespace ui {

dragging_tool::dragging_tool(int position_tolerance, float time_tolerance)
:   m_position_tolerance(position_tolerance)
,   m_time_tolerance(time_tolerance)
,   m_is_finished(false)
,   m_is_active(false)
,   m_hot_item(0)
{
}

void dragging_tool::set_hot_item(void const* item)
{
    m_hot_item = item;
}

void const* dragging_tool::hot_item() const
{
    return m_hot_item;
}

vector2i dragging_tool::start_position() const
{
    return m_start_position;
}

vector2i dragging_tool::delta() const
{
    //assert(m_is_active);
    return m_end_position - m_start_position;
}

vector2i dragging_tool::end_position_and_clear()
{
    assert(!m_is_active);
    m_is_finished = false;
    return m_end_position;
}

bool dragging_tool::is_active() const
{
    if (!m_is_active)
        return false;

    // Active, but lets check tolerances.
    bool delta_below_tolerance = length(static_vector_cast<float>(delta()))
        < static_cast<float>(m_position_tolerance);
    bool time_below_tolerance = true;
        //(m_current_time - m_start_time) < m_time_tolerance;

    if (m_is_active && delta_below_tolerance && time_below_tolerance)
        return false;

    return m_is_active;
}

bool dragging_tool::is_finished() const
{
    return m_is_finished;
}

mouse_button_type dragging_tool::button() const
{
    return m_drag_button;
}

void dragging_tool::abort()
{
    m_is_active = false;
    m_is_finished = false;
}

void dragging_tool::handle_event(event& event)
{
    dispatch(event);
}

void dragging_tool::handle_mouse_button_down(mouse_button_down_event& event)
{
    if (event.consumed())
        return;

    // We dont support double click dragging.
    if (event.is_double_clicked())
        return;

    m_current_time = event.get_context().current_tick();

    //assert(!m_is_active);
    if (m_is_active)
    {
        // this will happen when stopping at debug breakpoint while dragging
        // tool is active.
        log("dragging tool malfunction, probaply becouse of window lost "
            "focus while dragging");
        return;
    }

    // Activate.
    start_drag(event);
    event.consume();
    invalidate();
}

void dragging_tool::handle_mouse_button_up(mouse_button_up_event& event)
{
    if (event.consumed())
        return;
    if (!m_is_active)
        return;

    m_current_time = event.get_context().current_tick();
    end_drag(event);
    event.consume();
    invalidate();
}

void dragging_tool::handle_mouse_move(mouse_move_event& event)
{
    if (event.consumed())
        return;
    if (!m_is_active)
        return;

    m_current_time = event.get_context().current_tick();
    m_end_position = event.position();
    event.consume();
    invalidate();
}

void dragging_tool::do_parse(config_node const&)
{
}

void dragging_tool::do_invalidate()
{
    if (parent())
        parent()->invalidate();
}

void dragging_tool::start_drag(mouse_button_event& event)
{
    m_is_active = true;
    m_is_finished = false;
    m_drag_button = event.button();
    m_start_position = event.position();
    m_end_position = m_start_position;

    m_start_time = event.get_context().current_tick();
}

void dragging_tool::end_drag(mouse_button_event& event)
{
    // Mark as finished dragging only if dragging was beyond the tolerances.
    m_is_finished = is_active() ? true : false;

    m_is_active = false;
    m_end_position = event.position();
}

}} // namespace pt::ui
