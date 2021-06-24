#ifndef PT_UI_DRAGGING_TOOL_H
#define PT_UI_DRAGGING_TOOL_H

#include "element.h"
#include "event_handler.h"
#include "mouse_event.h"
#include "pt/vector.h"

namespace pt { namespace ui {

/// Class for managing dragging operations.
/// \internal \note Do NOT use is_active() inside the implementation of this
/// class, since it will return false if we are in tolerances, even if it
/// really is active.
class dragging_tool : public element, public event_handler
{
public:
    /// Does not count mouse movement less than \a position_tolerance as
    /// dragging until \a time_tolerance expires.
    dragging_tool(int position_tolerance, float time_tolerance);

    /// Any user settable item as the focus for this dragging operation.
    void set_hot_item(void const* item);
    void const* hot_item() const;

    /// \return Start position for currently active or most recent dragging
    /// operation.
    vector2i start_position() const;
    /// \return Delta position relative to the start_position() of currently
    /// active dragging operation.
    vector2i delta() const;
    /// Gets the end position and sets internals for this operation as
    /// non-finished. \return End position for most recent dragging operation.
    vector2i end_position_and_clear();

    /// \return \c true if this dragging operation is active.
    bool is_active() const;
    /// \return \c true if this dragging operation is finished.
    bool is_finished() const;
    /// \return The button which currently active or most recent operation
    /// is/was associated with.
    mouse_button_type button() const;

    /// Aborts active dragging operation.
    void abort();

public: // interface required by ui::event_handler
    void handle_event(event& event);

private: // ui::event_handler virtual function implementations
    void handle_mouse_button_down(mouse_button_down_event& event);
    void handle_mouse_button_up(mouse_button_up_event& event);
    void handle_mouse_move(mouse_move_event& event);

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

private: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // helper functions
    void start_drag(mouse_button_event& event);
    void end_drag(mouse_button_event& event);

public: // data members
    vector2i m_start_position;
    vector2i m_end_position;
    bool m_is_finished;
    bool m_is_active;
    mouse_button_type m_drag_button;

    uint32 m_start_time;
    uint32 m_current_time;
    int m_position_tolerance;
    float m_time_tolerance;

    void const* m_hot_item;
};

}} // namespace pt::ui

#endif
