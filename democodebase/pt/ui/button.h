#ifndef PT_UI_BUTTON_H
#define PT_UI_BUTTON_H

#include "bounded_element.h"
#include "event_handler.h"
#include "text_field.h"

namespace pt { namespace ui {

/// Button UI element.
class button : public bounded_element, public event_handler
{
public:
    /// Constructs button with zero bounds. Remember to call set_bounds().
    button();
    button(box2i const& bounds);

    void set_text(std::string const& text);
    std::string const& text() const;

	void set_font(std::shared_ptr<d3d9::font> font);

public: // ui::event_handler virtual function implementations
    void handle_event(event& event);

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

public: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // bounded_element virtual function overrides
    void on_set_bounds();

private: // data members
    text_field m_text_field;
};

}} // namespace pt::ui

#endif
