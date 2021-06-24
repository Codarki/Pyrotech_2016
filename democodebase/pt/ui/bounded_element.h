#ifndef PT_UI_BOUNDED_ELEMENT_H
#define PT_UI_BOUNDED_ELEMENT_H

#include "element.h"
#include "child_frame.h"

namespace pt { namespace ui {

/// Abstract UI element with 2d bounds.
class bounded_element : public element, public simple_bounded_frame
{
public:
    bounded_element();
    bounded_element(box2i const& bounds);

protected: // simple_bounded_frame virtual function overrides
    void set_bounds_called();

protected: // hook functions
    void on_set_bounds();
};

}} // namespace pt::ui

#endif
