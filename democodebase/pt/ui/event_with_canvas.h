#ifndef PT_UI_EVENT_WITH_CANVAS_H
#define PT_UI_EVENT_WITH_CANVAS_H

#include "canvas.h"

namespace pt { namespace ui {

/// Abstract UI event that has a canvas attached to it.
class event_with_canvas
{
public:
    event_with_canvas(ui::canvas& canvas);
    ui::canvas const& canvas() const;
    ui::canvas& canvas();

	event_with_canvas(const event_with_canvas&) = delete;
	event_with_canvas& operator=(const event_with_canvas&) = delete;

private: // data members
    ui::canvas& m_canvas;
};

/// Draws a rectangle using canvas from \a event.
/// \relatesalso event_with_canvas \relatesalso canvas
void draw_rectangle(event_with_canvas const& event, box2f const& area,
    vector4f const& color);
/// Draws a rectangle using canvas from \a event.
/// \relatesalso event_with_canvas \relatesalso canvas
void draw_rectangle(event_with_canvas const& event, vector2f const& min,
    vector2f const& max, vector4f const& color);

/// Fills a rectangle using canvas from \a event.
/// \relatesalso event_with_canvas \relatesalso canvas
void fill_rectangle(event_with_canvas const& event, box2f const& area,
    vector4f const& color);
/// Fills a rectangle using canvas from \a event.
/// \relatesalso event_with_canvas \relatesalso canvas
void fill_rectangle(event_with_canvas const& event, vector2f const& min,
    vector2f const& max, vector4f const& color);

/// Draws a line using canvas from \a event.
/// \relatesalso event_with_canvas \relatesalso canvas
void draw_line(event_with_canvas const& event, vector2f const& start,
    vector2f const& end, vector4f const& color);

}} // namespace pt::ui

#endif
