#ifndef CONTENT_SEQUENCES_AUDIO_H
#define CONTENT_SEQUENCES_AUDIO_H

#include "simple_composite_sequence.h"
#include "external/fmod_impl/sound.h"
#include "pt/vector.h"
#include "pt/path_fwd.h"

namespace content { namespace sequences {

struct audio_channel_data
{
    std::vector<float> data; // [0, 1]
};

class audio : public simple_composite_sequence
{
public:
    audio(
        std::shared_ptr<fmod_impl::system> system,
        pt::path const& path,
        pt::config_node const& config,
        animatable_value<float> spectrum_scale,
        animatable_value<float> spectrum_min_threshold,
        animatable_value<float> spectrum_max_threshold
    );

    void update_spectrum(float time);

    std::vector<float> const& left_amplitude() const;
    std::vector<float> const& right_amplitude() const;
    std::vector<float> const& left_decibel() const;
    std::vector<float> const& right_decibel() const;
    std::vector<float> total_energy() const; // left and right

    // Size 24. First 16 is [0, 3150] hz. Amplitudes.
    std::vector<float> left_energy() const;
    // Size 24. First 16 is [0, 3150] hz. Amplitudes.
    std::vector<float> right_energy() const;

    float entry_hz() const;
    size_t bark_index(float hz) const;

    bool is_playing() const;

    vector4f vector4f_spectrum(float time) const;

private: // simple_composite_sequence virtual function overloads
    std::shared_ptr<pt::d3d9::texture_2d> on_draw(rendertarget& target,
        float time, std::shared_ptr<pt::d3d9::texture_2d> child_result);

private: // data members
    std::shared_ptr<fmod_impl::sound> m_sound;
    std::weak_ptr<fmod_impl::system> m_system;
    FMOD::Channel* m_channel;
    float m_time;
    bool m_is_playing;
    animatable_value<float> m_spectrum_scale;
    animatable_value<float> m_spectrum_min_threshold;
    animatable_value<float> m_spectrum_max_threshold;

    std::vector<float> m_left_amplitude;
    std::vector<float> m_right_amplitude;
    std::vector<float> m_left_decibel;
    std::vector<float> m_right_decibel;
    std::vector<float> m_left_bark_max_amplitude;
    std::vector<float> m_right_bark_max_amplitude;
    float m_frequency;
    //float m_total_energy;
    std::vector<float> m_total_energy;
    std::vector<float> m_left_energy; // average amplitude for bark
    std::vector<float> m_right_energy; // average amplitude for bark
    float m_entry_hz;
};

float amplitude_to_db(float amplitude);

}} // namespace content::sequences

#endif
