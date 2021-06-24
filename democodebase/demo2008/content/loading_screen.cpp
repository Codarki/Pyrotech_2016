#include "loading_screen.h"
#include "user_abort.h"
#include "sequences/draw_image.h"
#include "sequences/image.h"
#include "sequences/rendertarget.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/ui/input_event.h"
#include "pt/ui/invalidatable.h"
#include "pt/any_error.h"
#include "pt/animatable_value.h"
#include "pt/config_node.h"
#include "pt/parameter_node.h"
#include "pt/parameter_collection.h"

namespace content {

loading_screen::loading_screen(d3d9::graphics_device& device,
    ui::invalidatable* invalidatable, vector2i resolution)
:   d3d9::device_reference(device)
,   m_bar(device)
,   m_invalidatable(invalidatable)
,   m_fading(false)
,   m_user_aborted(false)
,   m_last_draw_time(0.0f)
{
    assert(m_invalidatable);

	std::shared_ptr<sequences::composite_sequence> image;
    {
        config_node config("data/loading_screen.xml");

        std::string image_path;
        config.child("loading_screen")->parse_child("image", image_path);

        image.reset(new sequences::image(device, "loading_screen_image",
            image_path));

        m_bar = progress_bar(device,
            *config.child("loading_screen")->child("progress_bar"),
            resolution);

        config.child("loading_screen")->parse_optional_child(
            "fade_duration", m_fade_duration, 1.0f);
    }

    config_node config("draw_image", "");

    //
    config.insert_child(config_node("name", "draw_loading_screen"));
    config.insert_child(config_node("alpha", ""));
    config.child("alpha")->insert_child(
        config_node("curve", "loading_screen_alpha"));

    //
    //config_node alpha_config;
    //alpha_config.set_key("alpha");

    //write_child(alpha_config, "curve", std::string("loading_screen_alpha"));
    //config.insert_child(alpha_config);

    m_alpha.reset(new basic_value_source<float>(0.0f));

    std::shared_ptr<parameter_node> curve_node(new parameter_node("curves"));
    curve_node->children().add(std::shared_ptr<parameter_node>(
        new parameter_node("loading_screen_alpha",
        std::shared_ptr<animatable_value<float>>(new animatable_value<float>(m_alpha)))));

    parameter_collection parameters;
    parameters.add(curve_node);

    m_sequence.reset(
        new sequences::draw_image(device, image, config, parameters));
}

loading_screen::~loading_screen()
{
}

void loading_screen::fade_in()
{
    m_fading = true;
    m_invalidatable->invalidate();

    m_fade_start_time = GetTickCount() / 1000.0f;
    m_fade_end_time = std::numeric_limits<float>::max();
    m_pump.run(*this);

    if (m_user_aborted)
        throw user_abort();
}

void loading_screen::fade_out()
{
    m_fading = true;
    m_invalidatable->invalidate();

    m_fade_end_time = GetTickCount() / 1000.0f + m_fade_duration;
    m_pump.run(*this);

    if (m_user_aborted)
        throw user_abort();
}

void loading_screen::handle_event(ui::event& event)
{
    dispatch(event);
}

void loading_screen::handle_close(ui::close_event&)
{
    m_user_aborted = true;
    m_pump.quit();
}

void loading_screen::handle_key_down(ui::key_down_event& event)
{
    if (event.key_code() == VK_ESCAPE)
    {
        m_user_aborted = true;
        m_pump.quit();
    }
}

namespace {
    float do_pow_4_curve(float t)
    {
        if (t < 0.0f || t > 1.0f)
            return t;

        return std::pow(t, 4.0f);
    }
}

void loading_screen::handle_draw(ui::draw_event&)
{
    float time = GetTickCount() / 1000.0f;
    float local_time = time - m_fade_start_time;

    m_last_draw_time = time;

    DWORD flags = D3DCLEAR_TARGET;
    vector4f clear_color(0, 0, 0, 0);
    d3d9::clear(device(), flags, clear_color, 1.0f, 0);

    if (m_fading)
    {
        float duration = m_fade_end_time - m_fade_start_time;
        float fade_in = local_time / m_fade_duration;
        float fade_out = (duration - local_time) / m_fade_duration;
        float alpha = std::min(1.0f, std::min(fade_in, fade_out));

        if (local_time > m_fade_duration
            && time < (m_fade_end_time - m_fade_duration))
        {
            // Quit fade in.
            m_fading = false;
            m_pump.quit();
        }
        if (time > m_fade_end_time)
        {
            // Quit fade out.
            m_fading = false;
            m_pump.quit();
        }

        if (alpha < 0.0f)
            return;

        alpha = do_pow_4_curve(alpha);
        m_alpha->set(alpha);

        sequences::rendertarget target;
        m_sequence->draw(target, time);

    }
    else
    {
        m_alpha->set(1.0f);

        sequences::rendertarget target;
        m_sequence->draw(target, time);

        m_bar.draw();
    }
    //m_bar.draw();
}

void loading_screen::advance()
{
    m_bar.add_tick();

    // Invalidate at 60hz.
    float time = GetTickCount() / 1000.0f;
    if (time - m_last_draw_time > 1.0f / 60.0f)
        m_invalidatable->invalidate();

    m_pump.run_pending_messages();

    if (m_user_aborted)
        throw user_abort();
}

bool loading_screen::processes_on_idle() const
{
    return true;
}

float loading_screen::next_processing_delta_time() const
{
    return 1.0f / 60.0f;
}

void loading_screen::on_idle()
{
    // Invalidate at 60hz.
    float time = GetTickCount() / 1000.0f;
    if (time - m_last_draw_time > 1.0f / 60.0f)
        m_invalidatable->invalidate();
}

} // namespace content
