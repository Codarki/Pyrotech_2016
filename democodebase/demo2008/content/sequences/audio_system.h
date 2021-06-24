#ifndef CONTENT_SEQUENCES_AUDIO_SYSTEM_H
#define CONTENT_SEQUENCES_AUDIO_SYSTEM_H

#include "simple_composite_sequence.h"

namespace fmod_impl {
    class system;
}

namespace content { namespace sequences {

using namespace pt;

class audio;

/// Abstract base class for composition of sequences.
class audio_system : public simple_composite_sequence
{
public:
	audio_system(config_node const& config,
        parameter_collection const& parameters);

    audio& get_audio();

private: // simple_composite_sequence virtual function overloads
	std::shared_ptr<d3d9::texture_2d> on_draw(rendertarget& target,
        float time, std::shared_ptr<d3d9::texture_2d> child_result);

private: // data members
    std::shared_ptr<fmod_impl::system> m_system;
    std::shared_ptr<audio> m_audio;
};

}} // namespace content::sequences

#endif
