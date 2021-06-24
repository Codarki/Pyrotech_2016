#ifndef CONTENT_SEQUENCE_BUILDER_H
#define CONTENT_SEQUENCE_BUILDER_H

#include "sequences/composite_sequence_collection.h"
#include "pt/boost/noncopyable.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/parameter_collection_fwd.h"

namespace pt {
    class progress_report;
    class config_node;
}

namespace pt { namespace d3d9 { namespace rendering {
    class material_cache;
}}}

namespace content { namespace sequences {
    class audio_system;
    class composite_sequence;
    class scene_cache;
}}

namespace content {

using namespace pt;

class sequence_builder : boost::noncopyable
{
public:
    sequence_builder(
        d3d9::graphics_device& device,
        config_node const& config,
        parameter_collection const& parameters,
        d3d9::rendering::material_cache& materials,
        sequences::scene_cache& scene_cache,
        progress_report* progress,
        bool only_serialize);

    sequences::composite_sequence_collection const& sequences();
    std::shared_ptr<sequences::audio_system> audio_system_sequence() const;

private:
    void build_recursive(
        config_node const& config,
        parameter_collection const& parameters,
        bool only_serialize);

private:
    d3d9::graphics_device& m_device;
    progress_report* m_progress;
    d3d9::rendering::material_cache& m_materials;

    sequences::scene_cache* m_scene_cache;
    sequences::composite_sequence_collection m_sequences;
    std::shared_ptr<sequences::audio_system> m_audio_system_sequence;
};

} // namespace content

#endif
