#ifndef PT_UI_CANVAS_H
#define PT_UI_CANVAS_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/d3d9/texture_2d_fwd.h"
#include "pt/box.h"
#include "pt/boost/noncopyable.h"
#include "pt/std/memory.h"
#include "pt/std/stack.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {
    class font;
}} // namespace pt::d3d9

namespace pt { namespace ui {

/// Canvas for a window. Performs automatic conversion of child viewport
/// (screen space) coordinates to parent viewport.
class canvas
{
public: // type definitions
    /// Viewport within canvas.
    struct viewport
    {
        viewport(vector2i const& min, vector2i const& max);
        vector2i min;
        vector2i max;
    };

    /// Scoped viewport within canvas.
    class scoped_viewport : boost::noncopyable
    {
    public:
        scoped_viewport(canvas& canvas, viewport const& viewport);
        scoped_viewport(canvas& canvas, vector2i const& min,
            vector2i const& max);
        ~scoped_viewport();

    private: // data members
        canvas& m_canvas;
    };

public:
	canvas(std::shared_ptr<d3d9::graphics_device> device);

    void draw_rectangle(box2f const& area, vector4f const& color) const;
    void draw_rectangle(vector2f const& min, vector2f const& max,
        vector4f const& color) const;

    void fill_rectangle(box2f const& area, vector4f const& color) const;
    void fill_rectangle(vector2f const& min, vector2f const& max,
        vector4f const& color) const;
    void fill_rectangle(box2f const& area,
        std::shared_ptr<d3d9::texture_2d> texture) const;
    void fill_rectangle(vector2f const& min, vector2f const& max,
        std::shared_ptr<d3d9::texture_2d> texture) const;

    void fill(std::shared_ptr<d3d9::texture_2d> texture) const;

    void draw_line(vector2f const& start, vector2f const& end,
        vector4f const& color) const;

    void draw_text(d3d9::font const& font, std::string const& text,
        box2i const& area, vector4i const& rgba, int flags) const;

    void push_view(viewport const& viewport);
    void pop_view();

    std::shared_ptr<d3d9::graphics_device const> device() const;
    std::shared_ptr<d3d9::graphics_device> device();

private: // data members
    std::stack<viewport> m_viewports;
    std::shared_ptr<d3d9::graphics_device> m_device;
};

}} // namespace pt::ui

#endif
