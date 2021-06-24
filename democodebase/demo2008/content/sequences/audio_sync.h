#ifndef CONTENT_SEQUENCES_AUDIO_SYNC_H
#define CONTENT_SEQUENCES_AUDIO_SYNC_H

#include "pt/boost/smart_ptr.h"
#include "pt/boost/shared_array.h"
#include "external/fmod_impl/sound.h"
#include "pt/std/memory.h"


namespace content { namespace sequences {

using namespace pt;

class audio_sync 
{
public:
    audio_sync(std::shared_ptr<fmod_impl::system> fmod_system, int sampling_rate, int num_fourier_values);
    
    float calc_spectrum();
    
private: // data members
    int m_sampling_rate;
    int m_num_fourier_values;
    float m_step_hz;
    std::shared_ptr<fmod_impl::system> m_fmod_system;

    boost::shared_array<float> m_amplitudes_left;
    boost::shared_array<float> m_amplitudes_right;
};

}} // namespace content::sequences

#endif
