#ifndef PT_UI_BOUNDED_EVENT_H
#define PT_UI_BOUNDED_EVENT_H

#include "event.h"
#include "pt/box.h"
#include "pt/std/stack.h"

namespace pt { namespace ui {

/// Abstract UI event with stack of 2d bounds.
///
/// Idea of this class is that for all handlers and child handlers, the
/// bounds is in range [(0,0), (size_x, size_y)]. This encapsulates
class bounded_event : public event
{
public:
    bounded_event(box2i const& bounds);

    /// \return Bounds of the top of the stack.
    box2i const& bounds() const;

    /// Pushes \a child_bounds into stack after it has been clamped to the top.
    void push(box2i const& child_bounds);
    /// Pops the top from the stack.
    void pop();

private: // data members
    std::stack<box2i> m_bounds;
};

}} // namespace pt::ui

#endif
