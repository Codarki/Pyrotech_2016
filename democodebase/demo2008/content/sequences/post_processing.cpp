#include "post_processing.h"
#include "rendertarget.h"
#include "../utility.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/config_node.h"
#include "pt/math_utils.h"
#include "pt/parameter_node.h"

namespace content { namespace sequences {

// post_processing

post_processing::post_processing(d3d9::graphics_device& device,
    config_node const& config, parameter_collection const& parameters)
: m_basic(config, parameters)
, m_time(0.0f)
{
    m_post_processing.reset(
        new d3d9::rendering::post_processing(device, parameters));

    assert(config.key() == "post_processing");

    std::shared_ptr<parameter_node> curves = *parameters.child("curves");
    m_clear_color = create_curve_value<vector3f>(
        *config.child("clear_color"), curves);
}

string_wrapper const& post_processing::name() const
{
    return m_basic.name();
}

std::shared_ptr<d3d9::texture_2d> post_processing::draw(rendertarget& target, float time)
{
    m_time = time;
    m_post_processing->update(time);

    {
        // Draw child sequences into post processing input texture.
        d3d9::rendering::scoped_post_processing scoped_input(*m_post_processing);

        d3d9::graphics_device& device = m_post_processing->device();

        // Clear.
        DWORD flags = D3DCLEAR_TARGET;
        //vector4f clear_color(0, 0, 0.25f, 0);
        //vector4f clear_color(0, 0, 0, 0);

        vector4f clear_color(m_clear_color->value_at(m_time), 1.0f);
        clear_color[0] = pt::clamp(clear_color[0], 0.0f, 1.0f);
        clear_color[1] = pt::clamp(clear_color[1], 0.0f, 1.0f);
        clear_color[2] = pt::clamp(clear_color[2], 0.0f, 1.0f);
        clear_color[3] = pt::clamp(clear_color[3], 0.0f, 1.0f);

        d3d9::clear(device, flags, clear_color, 1.0f, 0);

        d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_CCW);
        d3d9::scoped_render_state zenable(device, D3DRS_ZENABLE, TRUE);

        // Draw
        rendertarget pp_target(device, m_post_processing->input_color());

        std::shared_ptr<d3d9::texture_2d> result = m_basic.draw(pp_target, time);
        if (result)
        {
        }
    }

    // Draw post processing from input to \a target.
    scoped_optional_rendertarget scoped_target(target);
    m_post_processing->execute();

    return m_post_processing->input_color();
}

float post_processing::start_time() const
{
    return m_basic.start_time();
}

float post_processing::end_time() const
{
    return m_basic.end_time();
}

void post_processing::set_override_view(d3d9::rendering::item* view)
{
    m_basic.set_override_view(view);
}

void post_processing::link_inputs(composite_sequence_collection const& sequences)
{
    m_basic.link_inputs(sequences);
}

}} // namespace content::sequences
