#ifndef PT_UI_ELEMENTS_RECTANGLE_H
#define PT_UI_ELEMENTS_RECTANGLE_H

#include "../bounded_element.h"
#include "../event_handler.h"

namespace pt { namespace ui { namespace elements {

/// Rectangle UI element.
class rectangle : public bounded_element, public event_handler
{
public:
    rectangle();
    rectangle(box2i const& bounds);

    void set_border_color(vector4f const& color);

public: // ui::event_handler virtual function implementations
    void handle_event(event& event);

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

public: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // bounded_element virtual function overrides
    void on_set_bounds();

private: // data members
    vector4f m_border_color;
};

}}} // namespace pt::ui::elements

#endif
