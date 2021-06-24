#include "progress_bar.h"
#include "sequences/image.h"
#include "sequences/rendertarget.h"
#include "pt/d3d9/fill_rectangle.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/config_node.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/math_utils.h"
#include "pt/vector_utility.h"

namespace content {

progress_bar::progress_bar(d3d9::graphics_device& device)
: m_device(&device)
{
}

progress_bar::progress_bar(d3d9::graphics_device& device,
    config_node const& config, vector2i resolution)
: m_device(&device)
, m_current_tick(0)
{
    assert(config.key() == "progress_bar");

    m_image.reset(new sequences::image(device, "progress_bar_image",
        config.child("image")->value()));
    m_mask.reset(new sequences::image(device, "progress_bar_mask",
        config.child("mask")->value()));

    vector2i reference_resolution;
    vector2i center;
    config.parse_child("reference_resolution", reference_resolution);
    config.parse_child("center", center);

    center[0] = center[0] * resolution[0] / reference_resolution[0];
    center[1] = center[1] * resolution[1] / reference_resolution[1];

    vector2i half_size = m_mask->texture()->resolution() / 2;
    m_min = center - half_size;
    m_size = m_mask->texture()->resolution();

    if (m_min[0] < 0)
        m_min[0] = 0;

    if (m_size[0] > resolution[0])
        m_size[0] = resolution[0];

    config.parse_child("total_ticks", m_total_ticks);
}

progress_bar::~progress_bar()
{
    log(format("total progress ticks: %1%") % m_current_tick);
}

void progress_bar::draw()
{
    sequences::rendertarget target;

	std::shared_ptr<d3d9::texture_2d> texture = m_image->draw(target, 0);
    std::shared_ptr<d3d9::texture_2d> mask_texture = m_mask->draw(target, 0);

    float progress = m_current_tick / static_cast<float>(m_total_ticks);
    progress = clamp(progress, 0.0f, 1.0f);

    vector2f min = static_vector_cast<float>(m_min);
    vector2f size = static_vector_cast<float>(m_size);

    vector2f min_uv(0, 0);
    vector2f max_uv(1, 1);

    {
        vector2f max = min + size;

        d3d9::fill_rectangle(*m_device, min, max, min_uv, max_uv,
            texture->native(), true);
    }

    {
        size[0] *= progress;
        max_uv[0] *= progress;

        vector2f max = min + size;

        d3d9::fill_rectangle(*m_device, min, max, min_uv, max_uv,
            mask_texture->native(), true);
    }
}

void progress_bar::set_tick(size_t tick)
{
    m_current_tick = tick;
}

void progress_bar::add_tick()
{
    ++m_current_tick;
}

} // namespace content
