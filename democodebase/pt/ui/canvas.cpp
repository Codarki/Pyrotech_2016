#include "canvas.h"
#include "pt/d3d9/draw_rectangle.h"
#include "pt/d3d9/fill_rectangle.h"
#include "pt/d3d9/font.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/std/algorithm.h"
#include <yvals.h>
#include <xutility>

namespace pt { namespace ui {
namespace {
    D3DVIEWPORT9 top_d3d_view(std::stack<canvas::viewport> const& stack)
    {
        vector2i range = stack.top().max - stack.top().min;
        D3DVIEWPORT9 view;
        view.X = stack.top().min[0];
        view.Y = stack.top().min[1];
        view.Width = range[0];
        view.Height = range[1];
        view.MinZ = 0.0f;
        view.MaxZ = 1.0f;
        return view;
    }
} // anonymous namespace

canvas::viewport::viewport(vector2i const& min, vector2i const& max)
:   min(min)
,   max(max)
{
}

canvas::scoped_viewport::scoped_viewport(canvas& canvas,
    viewport const& viewport)
:   m_canvas(canvas)
{
    m_canvas.push_view(viewport);
}

canvas::scoped_viewport::scoped_viewport(canvas& canvas, vector2i const& min,
    vector2i const& max)
:   m_canvas(canvas)
{
    m_canvas.push_view(canvas::viewport(min, max));
}

canvas::scoped_viewport::~scoped_viewport()
{
    m_canvas.pop_view();
}

// canvas

canvas::canvas(std::shared_ptr<d3d9::graphics_device> device)
:   m_device(device)
{
}

void canvas::draw_rectangle(box2f const& area, vector4f const& color) const
{
    draw_rectangle(min_position(area), max_position(area), color);
}

void canvas::draw_rectangle(vector2f const& min, vector2f const& max,
    vector4f const& color) const
{
    d3d9::scoped_viewport scoped_viewport(*m_device,
        top_d3d_view(m_viewports));
    d3d9::draw_rectangle(*m_device, min, max, color);
}

void canvas::fill_rectangle(box2f const& area, vector4f const& color) const
{
    fill_rectangle(min_position(area), max_position(area), color);
}

void canvas::fill_rectangle(vector2f const& min, vector2f const& max,
    vector4f const& color) const
{
    d3d9::scoped_viewport scoped_viewport(*m_device,
        top_d3d_view(m_viewports));
    d3d9::fill_rectangle(*m_device, min, max, color);
}

void canvas::fill_rectangle(box2f const& area,
    std::shared_ptr<d3d9::texture_2d> texture) const
{
    fill_rectangle(min_position(area), max_position(area), texture);
}

void canvas::fill_rectangle(vector2f const& min, vector2f const& max,
    std::shared_ptr<d3d9::texture_2d> texture) const
{
    d3d9::scoped_viewport scoped_viewport(*m_device,
        top_d3d_view(m_viewports));
    d3d9::fill_rectangle(*m_device, min, max, texture->native());
}

void canvas::fill(std::shared_ptr<d3d9::texture_2d> texture) const
{
    fill_rectangle(static_vector_cast<float>(m_viewports.top().min),
        static_vector_cast<float>(m_viewports.top().max), texture);
}

void canvas::draw_line(vector2f const& start, vector2f const& end,
    vector4f const& color) const
{
    d3d9::scoped_viewport scoped_viewport(*m_device,
        top_d3d_view(m_viewports));
    d3d9::draw_line(*m_device, start, end, color);
}

void canvas::draw_text(d3d9::font const& font, std::string const& text,
    box2i const& area, vector4i const& rgba, int flags) const
{
    d3d9::scoped_viewport scoped_viewport(*m_device,
        top_d3d_view(m_viewports));
    d3d9::draw_text(font, text, area, rgba, static_cast<DWORD>(flags));
}

void canvas::push_view(viewport const& viewport)
{
    if (m_viewports.empty())
    {
        m_viewports.push(viewport);
    }
    else
    {
        vector2i min;
        vector2i max;
        min[0] = std::max(m_viewports.top().min[0], viewport.min[0]);
        min[1] = std::max(m_viewports.top().min[1], viewport.min[1]);
        max[0] = std::min(m_viewports.top().max[0], viewport.max[0]);
        max[1] = std::min(m_viewports.top().max[1], viewport.max[1]);

        m_viewports.push(canvas::viewport(min, max));
    }
}

void canvas::pop_view()
{
    m_viewports.pop();
}

std::shared_ptr<d3d9::graphics_device const> canvas::device() const
{
    return m_device;
}

std::shared_ptr<d3d9::graphics_device> canvas::device()
{
    return m_device;
}

}} // namespace pt::ui
