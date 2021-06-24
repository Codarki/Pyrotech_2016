#include "sequence_builder.h"
#include "sequences/audio_system.h"
#include "sequences/animated_texture.h"
#include "sequences/basic_composite_sequence.h"
#include "sequences/color_fade.h"
#include "sequences/cloud_texture.h"
#include "sequences/custom_effect.h"
#include "sequences/depth_buffer.h"
#include "sequences/draw_image.h"
#include "sequences/draw_to_texture.h"
#include "sequences/image.h"
#include "sequences/plasma.h"
#include "sequences/post_processing.h"
#include "sequences/scene_sequence.h"
#include "pt/config_node.h"
#include "pt/progress_report.h"

namespace content {

sequence_builder::sequence_builder(d3d9::graphics_device& device,
    config_node const& config, parameter_collection const& parameters,
    d3d9::rendering::material_cache& materials,
    sequences::scene_cache& scene_cache,
    progress_report* progress,
    bool only_serialize)
: m_device(device)
, m_materials(materials)
, m_progress(progress)
, m_scene_cache(&scene_cache)
{
    assert(config.key() == "sequences");

    // Build flat list of sequences
    build_recursive(config, parameters, only_serialize);

    // Link the sequences.
	typedef std::vector<std::shared_ptr<sequences::composite_sequence> >
        container;

    for (container::iterator i = m_sequences.get().begin(); i != m_sequences.get().end(); ++i)
    {
        (*i)->link_inputs(m_sequences);
    }
}

sequences::composite_sequence_collection const& sequence_builder::sequences()
{
    return m_sequences;
}

std::shared_ptr<sequences::audio_system> sequence_builder::audio_system_sequence() const
{
    return m_audio_system_sequence;
}

void sequence_builder::build_recursive(
    config_node const& config,
    parameter_collection const& parameters,
    bool only_serialize)
{
    for (config_node::const_iterator i = config.begin(); i != config.end();
        ++i)
    {
        build_recursive(*i, parameters, only_serialize);
    }

	std::shared_ptr<sequences::composite_sequence> sequence;
    std::string const config_key = config.key();

    if (config_key == "audio_system")
    {
        m_audio_system_sequence.reset(new sequences::audio_system(config, parameters));
        sequence = m_audio_system_sequence;
    }
    else if (config_key == "animated_texture")
    {
        sequence.reset(new sequences::animated_texture(config, parameters,
            m_device, m_materials));
    }
    else if (config_key == "composition")
    {
        sequence.reset(new sequences::basic_composite_sequence(config, parameters));
    }
    else if (config_key == "color_fade")
    {
        sequence.reset(
            new sequences::color_fade(m_device, config, parameters));
    }
    else if (config_key == "depth_buffer")
    {
        sequence.reset(new sequences::depth_buffer(m_device, config, parameters));
    }
    else if (config_key == "draw_image")
    {
        sequence.reset(
            new sequences::draw_image(m_device, config, parameters));
    }
    else if (config_key == "draw_to_texture")
    {
        sequence.reset(new sequences::draw_to_texture(m_device, config,
            parameters));
    }
    else if (config_key == "scene_sequence")
    {
        sequence.reset(
            new sequences::scene_sequence(m_device, config, parameters,
            m_materials, *m_scene_cache, m_progress, only_serialize));
    }
    else if (config_key == "post_processing")
    {
        sequence.reset(
            new sequences::post_processing(m_device, config, parameters));
    }
    else if (config_key == "image")
    {
        sequence.reset(new sequences::image(m_device, config));
    }
    else if (config_key == "plasma")
    {
        sequence.reset(new sequences::plasma(m_device, config, m_materials, parameters));
    }
    else if (config_key == "custom_effect")
    {
        sequence.reset(
            new sequences::custom_effect(m_device, config, parameters));
    }
    else if (config_key == "cloud_texture")
    {
        sequence.reset(
            new sequences::cloud_texture(config, parameters));
    }

    if (sequence)
    {
        m_sequences.get().push_back(sequence);
    }

    if (m_progress)
        m_progress->advance();
}

} // namespace content
