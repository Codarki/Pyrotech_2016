#ifndef CONTENT_TIMELINE_PLAYER_H
#define CONTENT_TIMELINE_PLAYER_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"
#include "pt/basic_value_source.h"
#include "pt/vector.h"
#include "pt/path_fwd.h"

namespace pt {
    class parameter_collection;
    class progress_report;
}

namespace pt { namespace d3d9 { namespace rendering {
    class material_cache;
    class item;
}}}

namespace content { namespace sequences {
    class composite_sequence;
}}

namespace content {

using namespace pt;

class timeline;

class timeline_player
{
public:
    timeline_player(d3d9::graphics_device& device, path const& path,
        d3d9::rendering::material_cache& materials,
        parameter_collection const& parameters,
        progress_report* progress,
        bool only_serialize);
    //timeline_player(d3d9::graphics_device& device,
    //    std::shared_ptr<timeline> timeline);

    void operator=(timeline_player const&) = delete;

    void draw(float time);
    //void update(float time);

    void set_view(d3d9::rendering::item* view);

private: // type definitions
    typedef std::vector<sequences::composite_sequence*> sequences;

private: // data members
    d3d9::graphics_device& m_device;
	std::shared_ptr<timeline> m_timeline;
    d3d9::rendering::item* m_view;
    float m_last_update_time;
    std::shared_ptr<basic_value_source<float>> m_audio_spectrum_segment_1;
    std::shared_ptr<basic_value_source<float>> m_audio_spectrum_segment_2;
    std::shared_ptr<basic_value_source<float>> m_audio_spectrum_segment_3;
    std::shared_ptr<basic_value_source<float>> m_audio_spectrum_segment_4;
    std::shared_ptr<basic_value_source<vector4f>> m_audio_spectrum_source;

    sequences m_active_sequences;
};

} // namespace content

#endif
