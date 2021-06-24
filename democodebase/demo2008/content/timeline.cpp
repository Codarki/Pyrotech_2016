#include "timeline.h"
#include "sequences/scene_cache.h"
#include "sequence_builder.h"
#include "sequences/scene_sequence.h"
#include "sequences/scene_cache.h"
#include "sequences/composite_sequence.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/parameter_collection.h"

namespace content {

using sequences::composite_sequence;

timeline::timeline(d3d9::graphics_device& device, path const& path,
    d3d9::rendering::material_cache& materials,
    parameter_collection const& parameters,
    progress_report* progress,
    bool only_serialize)
:   m_path(path)
{
    config_node config(path);
    parse(device, *config.child("sequences"), materials, parameters, progress, only_serialize);
}

std::vector<sequences::composite_sequence*> timeline::active_sequences_at(float time) const
{
    std::vector<composite_sequence*> result;

    for (container::const_iterator i = m_sequences.begin();
        i != m_sequences.end(); ++i)
    {
        composite_sequence& sequence = **i;
        float sequence_start = sequence.start_time();
        float sequence_end = sequence.end_time();
        if (time >= sequence_start && time < sequence_end)
        {
            result.push_back(&sequence);
            //result.insert(result.end(), i, i + 1);
        }
    }

    return result;
}

std::shared_ptr<sequences::audio_system> timeline::audio_system_sequence() const
{
    return m_audio_system_sequence;
}

void timeline::parse(d3d9::graphics_device& device, config_node const& config,
    d3d9::rendering::material_cache& materials,
    parameter_collection const& parameters,
    progress_report* progress,
    bool only_serialize)
{
    std::string input_name;

    for (config_node::const_iterator i = config.begin(); i != config.end();
        ++i)
    {
        if (i->key() == "timeline")
        {
            i->child("input")->parse_child("sequence", input_name);
        }
    }

    if (input_name.empty())
        throw any_error("invalid timeline chunk");

    sequences::scene_cache scene_cache;

    sequence_builder builder(device, config, parameters, materials,
        scene_cache, progress, only_serialize);

	typedef std::vector<std::shared_ptr<sequences::composite_sequence> >
        container;
    container const& sequences = builder.sequences().get();
    m_audio_system_sequence = builder.audio_system_sequence();

    for (container::const_iterator i = sequences.begin();
        i != sequences.end(); ++i)
    {
        if ((*i)->name() == input_name)
        {
            m_sequences.push_back(*i);
        }
    }
    if (m_sequences.empty())
        throw any_error("no inputs for timeline");
}

} // namespace content
