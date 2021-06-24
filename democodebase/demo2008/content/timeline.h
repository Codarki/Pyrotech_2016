#ifndef CONTENT_TIMELINE_H
#define CONTENT_TIMELINE_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/path.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt {
    class parameter_collection;
    class progress_report;
}

namespace pt { namespace d3d9 { namespace rendering {
    class material_cache;
}}}

namespace content { namespace sequences {
    class composite_sequence;
    class audio_system;
}}

namespace content {

class timeline
{
public:
    timeline(
        pt::d3d9::graphics_device& device,
        pt::path const& path,
        pt::d3d9::rendering::material_cache& materials,
        pt::parameter_collection const& parameters,
        pt::progress_report* progress,
        bool only_serialize);

    std::vector<sequences::composite_sequence*> active_sequences_at(float time) const;
    std::shared_ptr<sequences::audio_system> audio_system_sequence() const;

private: // type definitions
    typedef std::vector<std::shared_ptr<sequences::composite_sequence> > container;

private: // helper functions
    void parse(
        pt::d3d9::graphics_device& device,
        pt::config_node const& config,
        pt::d3d9::rendering::material_cache& materials,
        pt::parameter_collection const& parameters,
        pt::progress_report* progress,
        bool only_serialize);

private: // noncopyable
    timeline(timeline const&);
    timeline const& operator=(timeline const&);

private: // data members
    pt::path m_path;
    container m_sequences;
    std::shared_ptr<sequences::audio_system> m_audio_system_sequence;
};

} // namespace content

#endif
