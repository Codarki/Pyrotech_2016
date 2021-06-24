#ifndef PT_UI_ELEMENTS_LINE_H
#define PT_UI_ELEMENTS_LINE_H

#include "../bounded_element.h"
#include "../event_handler.h"

namespace pt { namespace ui { namespace elements {

/// Rectangle UI element.
class line : public bounded_element, public event_handler
{
public:
    line();
    line(vector2f const& from, vector2f const& to);
    line(vector2f const& from, vector2f const& to, box2i const& bounds);

    void set_from(vector2f const& from);
    void set_to(vector2f const& to);

    void set_color(vector4f const& color);

public: // ui::event_handler virtual function implementations
    void handle_event(event& event);

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

public: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // data members
    vector2f m_from;
    vector2f m_to;
    vector4f m_color;
};

}}} // namespace pt::ui::elements

#endif
