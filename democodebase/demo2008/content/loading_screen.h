#ifndef CONTENT_LOADING_SCREEN_H
#define CONTENT_LOADING_SCREEN_H

#include "pt/ui/event_handler.h"
#include "pt/d3d9/device_child.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/win32/message_pump.h"
#include "pt/progress_report.h"
#include "progress_bar.h"
#include "pt/basic_value_source.h"
#include "pt/std/memory.h"

namespace pt { namespace ui {
    class invalidatable;
}}

namespace content {

namespace sequences {
    class composite_sequence;
}

using namespace pt;

class loading_screen : public ui::event_handler, public progress_report,
    public win32::message_pump::listener, d3d9::device_reference
{
public:
    loading_screen(d3d9::graphics_device& device,
        ui::invalidatable* invalidatable, vector2i resolution);
    ~loading_screen();

    void fade_in();
    void fade_out();

public: // interface required by ui::event_handler
    void handle_event(ui::event& event);
    void handle_close(ui::close_event& event);
    void handle_key_down(ui::key_down_event& event);
    void handle_draw(ui::draw_event& event);

public: // interface required by progress_report
    void advance();

public: // interface required by win32::message_pump::listener
    bool processes_on_idle() const;
    float next_processing_delta_time() const;
    void on_idle();

private: // data members
	std::unique_ptr<sequences::composite_sequence> m_sequence;
    win32::message_pump m_pump;
    progress_bar m_bar;
    bool m_fading;
    bool m_user_aborted;
    float m_fade_duration;
    float m_fade_start_time;
    float m_fade_end_time;
    float m_last_draw_time;
    ui::invalidatable* m_invalidatable;

    std::shared_ptr<basic_value_source<float> > m_alpha;
};

} // namespace content

#endif
