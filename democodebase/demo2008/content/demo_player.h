#ifndef CONTENT_DEMO_PLAYER_H
#define CONTENT_DEMO_PLAYER_H

#include "pt/ui/event_handler.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/d3d9/rendering/material_cache.h"
#include "free_view.h"
#include "info_display.h"
#include "pt/com_ptr.h"
#include "pt/command_line.h"
#include "pt/basic_value_source.h"
#include "pt/std/memory.h"

namespace pt {
    class parameter_collection;
    class progress_report;
}

namespace pt { namespace d3d9 {
    template<typename T,int C>
    class formatted_text_source;
}}

namespace pt { namespace d3d9 { namespace rendering {
    class post_processing;
    class scene;
    class camera;
}}}

namespace pt { namespace ui {
    class invalidatable;
}}

namespace content {

namespace sequences {
    class post_processing_parameters;
}

class scene_player;
class timeline_player;
class spline_test;
class plasma;

using namespace pt;

class demo_player : public ui::event_handler
{
public:
    demo_player(d3d9::graphics_device& device, command_line* commands,
        progress_report& progress, ui::invalidatable* invalidatable);
    ~demo_player();

    void update();
    bool finished() const;
    void warm_up();

    void set_resolution(vector2i const& resolution);

public: // interface required by ui::event_handler
    void handle_event(ui::event& event);
    void handle_close(ui::close_event& event);
    void handle_draw(ui::draw_event& event);
    void handle_key_down(ui::key_down_event& event);

private: // helper functions
private: // data members
    //Sync parameters
    float m_sync_gap_before;
    float m_sync_gap_after;
    float m_sync_wait_time;
    float m_sync_max_value;
    float m_sync_min_value;
    bool m_sync_file_open;


    bool m_finished;
    bool m_show_info_display;
    size_t m_frame_count;

    d3d9::graphics_device& m_device;
	std::shared_ptr<scene_player> m_scene_player;
    std::shared_ptr<timeline_player> m_timeline_player;
    free_view_controller m_free_view;
	std::shared_ptr<d3d9::rendering::item> m_view;
    bool m_use_free_view;

    d3d9::rendering::item* m_current_view;
    command_line* m_commands;
    info_display m_info_display;
    bool m_paused;
    size_t m_camera_index;

    size_t m_global_start_time_ms;
    size_t m_global_current_time_ms;
    float m_local_current_time_s;

    std::unique_ptr<d3d9::rendering::material_cache> m_materials;
    std::shared_ptr<sequences::post_processing_parameters>
        m_post_processing_parameters;

    std::shared_ptr<d3d9::formatted_text_source<float, 3> > m_info_text;

    std::shared_ptr<plasma> m_plasma;

    ui::invalidatable* m_invalidatable;
};

} // namespace content

#endif
