#include "audio.h"
#include "pt/path.h"
#include "pt/std/map.h"
#include "external/fmod_impl/system.h"

namespace content { namespace sequences {

using namespace pt;

namespace {
    const int output_rate = 48000; // 44100;
    const float spectrum_range = (float)output_rate / 2.0f; // 0 to nyquist
    const int spectrum_size = 1024;
    //const float entry_hz = spectrum_range / (float)spectrum_size;

    const char* note[120] =
    {
        "C 0", "C#0", "D 0", "D#0", "E 0", "F 0", "F#0", "G 0", "G#0", "A 0", "A#0", "B 0",
        "C 1", "C#1", "D 1", "D#1", "E 1", "F 1", "F#1", "G 1", "G#1", "A 1", "A#1", "B 1",
        "C 2", "C#2", "D 2", "D#2", "E 2", "F 2", "F#2", "G 2", "G#2", "A 2", "A#2", "B 2",
        "C 3", "C#3", "D 3", "D#3", "E 3", "F 3", "F#3", "G 3", "G#3", "A 3", "A#3", "B 3",
        "C 4", "C#4", "D 4", "D#4", "E 4", "F 4", "F#4", "G 4", "G#4", "A 4", "A#4", "B 4",
        "C 5", "C#5", "D 5", "D#5", "E 5", "F 5", "F#5", "G 5", "G#5", "A 5", "A#5", "B 5",
        "C 6", "C#6", "D 6", "D#6", "E 6", "F 6", "F#6", "G 6", "G#6", "A 6", "A#6", "B 6",
        "C 7", "C#7", "D 7", "D#7", "E 7", "F 7", "F#7", "G 7", "G#7", "A 7", "A#7", "B 7",
        "C 8", "C#8", "D 8", "D#8", "E 8", "F 8", "F#8", "G 8", "G#8", "A 8", "A#8", "B 8",
        "C 9", "C#9", "D 9", "D#9", "E 9", "F 9", "F#9", "G 9", "G#9", "A 9", "A#9", "B 9"
    };

    const float notefreq[120] =
    {
        16.35f,   17.32f,   18.35f,   19.45f,    20.60f,    21.83f,    23.12f,    24.50f,    25.96f,    27.50f,    29.14f,    30.87f,
        32.70f,   34.65f,   36.71f,   38.89f,    41.20f,    43.65f,    46.25f,    49.00f,    51.91f,    55.00f,    58.27f,    61.74f,
        65.41f,   69.30f,   73.42f,   77.78f,    82.41f,    87.31f,    92.50f,    98.00f,   103.83f,   110.00f,   116.54f,   123.47f,
        130.81f,  138.59f,  146.83f,  155.56f,   164.81f,   174.61f,   185.00f,   196.00f,   207.65f,   220.00f,   233.08f,   246.94f,
        261.63f,  277.18f,  293.66f,  311.13f,   329.63f,   349.23f,   369.99f,   392.00f,   415.30f,   440.00f,   466.16f,   493.88f,
        523.25f,  554.37f,  587.33f,  622.25f,   659.26f,   698.46f,   739.99f,   783.99f,   830.61f,   880.00f,   932.33f,   987.77f,
        1046.50f, 1108.73f, 1174.66f, 1244.51f,  1318.51f,  1396.91f,  1479.98f,  1567.98f,  1661.22f,  1760.00f,  1864.66f,  1975.53f,
        2093.00f, 2217.46f, 2349.32f, 2489.02f,  2637.02f,  2793.83f,  2959.96f,  3135.96f,  3322.44f,  3520.00f,  3729.31f,  3951.07f,
        4186.01f, 4434.92f, 4698.64f, 4978.03f,  5274.04f,  5587.65f,  5919.91f,  6271.92f,  6644.87f,  7040.00f,  7458.62f,  7902.13f,
        8372.01f, 8869.84f, 9397.27f, 9956.06f, 10548.08f, 11175.30f, 11839.82f, 12543.85f, 13289.75f, 14080.00f, 14917.24f, 15804.26f
    };

    // Bark scale critical bands. The scale ranges from 1 to 24 and
    // corresponds to the first 24 critical bands of hearing.
    // https://en.wikipedia.org/wiki/Bark_scale
    const float bark_cut_off_frequency[24] =
    {
        100.0f, 200.0f, 300.0f, 400.0f, 510.0f, 630.0f, 770.0f, 920.0f,
        1080.0f, 1270.0f, 1480.0f, 1720.0f, 2000.0f, 2320.0f, 2700.0f,
        3150.0f, // 16
        3700.0f, 4400.0f, 5300.0f, 6400.0f, 7700.0f, 9500.0f,
        12000.0f, 15500.0f
    };

    // Key is hz, value is decibel change for humans. https://en.wikipedia.org/wiki/A-weighting
    // Taken from http://onlinelibrary.wiley.com/doi/10.1002/9780470869253.app4/pdf
    const std::map<float, float> a_weighting = {
        { 0.0f,   -70.0f }, // made up 0 hz
        { 10.0f,  -70.4f },
        { 13.0f,  -63.4f },
        { 16.0f,  -56.7f },
        { 20.0f,  -50.5f },
        { 25.0f,  -44.7f },
        { 32.0f,  -39.4f },
        { 40.0f,  -34.6f },
        { 50.0f,  -30.2f },
        { 63.0f,  -26.2f },
        { 80.0f,  -22.5f },
        { 100.0f, -19.1f },
        { 125.0f, -16.1f },
        { 160.0f, -13.4f },
        { 200.0f, -10.9f },
        { 250.0f, -8.6f },
        { 315.0f, -6.6f },
        { 400.0f, -4.8f },
        { 500.0f, -3.2f },
        { 630.0f, -1.9f },
        { 800.0f, -0.8f },
        { 1000.0f, 0.0f },
        { 1250.0f, 0.6f },
        { 1600.0f, 1.0f },
        { 2000.0f, 1.2f },
        { 2500.0f, 1.3f },
        { 3150.0f, 1.2f },
        { 4000.0f, 1.0f },
        { 5000.0f, 0.5f },
        { 6300.0f, -0.1f },
        { 8000.0f, -1.1f },
        { 10000.0f, -2.5f },
        { 12500.0f, -4.3f },
        { 16000.0f, -6.6f },
        { 20000.0f, -9.3f }
    };

    // Key is hz, value is decibel change for humans. https://en.wikipedia.org/wiki/A-weighting
    // Taken from http://onlinelibrary.wiley.com/doi/10.1002/9780470869253.app4/pdf
    const std::map<float, float> b_weighting = {
        { 0.0f,     -38.1f }, // made up 0 hz
        { 10.0f,    -38.1f },
        { 13.0f,    -33.2f },
        { 16.0f,    -28.5f },
        { 20.0f,    -24.2f },
        { 25.0f,    -20.4f },
        { 32.0f,    -17.1f },
        { 40.0f,    -14.2f },
        { 50.0f,    -11.6f },
        { 63.0f,     -9.3f },
        { 80.0f,     -7.4f },
        { 100.0f,    -5.6f },
        { 125.0f,    -4.2f },
        { 160.0f,    -3.0f },
        { 200.0f,    -2.0f },
        { 250.0f,    -1.3f },
        { 315.0f,    -0.8f },
        { 400.0f,    -0.5f },
        { 500.0f,    -0.3f },
        { 630.0f,    -0.1f },
        { 800.0f,     0.0f },
        { 1000.0f,    0.0f },
        { 1250.0f,    0.0f },
        { 1600.0f,    0.0f },
        { 2000.0f,   -0.1f },
        { 2500.0f,   -0.2f },
        { 3150.0f,   -0.4f },
        { 4000.0f,   -0.7f },
        { 5000.0f,   -1.2f },
        { 6300.0f,   -1.9f },
        { 8000.0f,   -2.9f },
        { 10000.0f,  -4.3f },
        { 12500.0f,  -6.1f },
        { 16000.0f,  -8.4f },
        { 20000.0f, -11.1f }
    };

    // Key is hz, value is decibel change. https://en.wikipedia.org/wiki/ITU-R_468_noise_weighting
    const std::map<float, float> itu_r_468_weighting = {
        { 0.0f,     -38.1f }, // made up 0 hz
        { 31.5f,    -29.9f },
        { 63.0f,    -23.9f },
        { 100.0f,   -19.8f },
        { 200.0f,   -13.8f },
        { 400.0f,   -7.8f },
        { 800.0f,   -1.9f },
        { 1000.0f,  0.0f },
        { 2000.0f,  5.6f },
        { 3150.0f,  9.0f },
        { 4000.0f,  10.5f },
        { 5000.0f,  11.7f },
        { 6300.0f,  12.2f },
        { 7100.0f,  12.0f },
        { 8000.0f,  11.4f },
        { 9000.0f,  10.1f },
        { 10000.0f, 8.1f },
        { 12500.0f, 0.0f },
        { 14000.0f, -5.3f },
        { 16000.0f, -11.7f },
        { 20000.0f, -22.2f },
        { 31500.0f, -42.7f }
    };

    float a_weighting_response(float hz)
    {
        auto it = a_weighting.lower_bound(hz);
        if (it == a_weighting.end())
            return -9.3f;

        return it->second;
    }

    float itu_r_468_weighting_response(float hz)
    {
        auto it = itu_r_468_weighting.lower_bound(hz);
        if (it == itu_r_468_weighting.end())
            return -42.7f;

        return it->second;
    }


    float decibel_offset(float hz)
    {
        // Average of A and ITU-R 468.
        //return (a_weighting_response(hz) + itu_r_468_weighting_response(hz)) * 0.5f;

        // Hack. Bring back lower hz.
        return pt::parameterize(-42.7f, 12.2f, -25.0f, 12.2f, itu_r_468_weighting_response(hz));
        //return itu_r_468_weighting_response(hz) * 0.75f + 5.0f;
    }

    // Decibels d = 10.0f * (float)log10(val) * 2.0f
    // Inverse: val = 10^(d / 20.0f)

    float db_to_amplitude(float db)
    {
        return std::pow(10.0f, db / 20.0f);
    }
} // anonymous

float amplitude_to_db(float amplitude)
{
    return 20.0f * std::log10(amplitude);
}


audio::audio(
    std::shared_ptr<fmod_impl::system> system,
    pt::path const& path,
    config_node const& config,
    animatable_value<float> spectrum_scale,
    animatable_value<float> spectrum_min_threshold,
    animatable_value<float> spectrum_max_threshold)
: simple_composite_sequence(config)
, m_channel(0)
, m_time(-1.0f)
, m_system(system)
, m_spectrum_scale(spectrum_scale)
, m_spectrum_min_threshold(spectrum_min_threshold)
, m_spectrum_max_threshold(spectrum_max_threshold)
, m_is_playing(false)
{
    m_sound = system->create_sound(path);
    m_left_amplitude.resize(spectrum_size);
    m_right_amplitude.resize(spectrum_size);
    m_left_decibel.resize(spectrum_size);
    m_right_decibel.resize(spectrum_size);
    m_left_bark_max_amplitude.resize(24);
    m_right_bark_max_amplitude.resize(24);
    m_left_energy.resize(24);
    m_right_energy.resize(24);
    m_total_energy.resize(2);

    m_entry_hz = spectrum_range / (float)spectrum_size;
}

void audio::update_spectrum(float time)
{
    std::shared_ptr<fmod_impl::system> system = m_system.lock();
    if (!system)
        return;

    if (!m_is_playing)
        return;

    auto spectrum_result_l = system->native()->getSpectrum(m_left_amplitude.data(), spectrum_size, 0, FMOD_DSP_FFT_WINDOW_HAMMING);
    if (spectrum_result_l != FMOD_OK)
        return;
    auto spectrum_result_r = system->native()->getSpectrum(m_right_amplitude.data(), spectrum_size, 1, FMOD_DSP_FFT_WINDOW_HAMMING);
    if (spectrum_result_r != FMOD_OK)
        return;

    // Normalize the data. Find the maximum value in the spectrum, and scale
    // all values in the array by 1 / max.
    //float left_max = 0.0001f;
    //float right_max = 0.0001f;
    //for (int count = 0; count < spectrum_size; ++count)
    //{
    //    if (m_left_spectrum[count] > left_max)
    //        left_max = m_left_spectrum[count];
    //    if (m_right_spectrum[count] > right_max)
    //        right_max = m_right_spectrum[count];
    //}
    //float inv_left_max = 1.0f / left_max;
    //float inv_right_max = 1.0f / right_max;
    //for (int count = 0; count < spectrum_size; ++count)
    //{
    //    m_left_spectrum[count] *= inv_left_max;
    //    m_right_spectrum[count] *= inv_right_max;
    //}

    // https://en.wikipedia.org/wiki/Bark_scale

    // Each bark band has own amplitude scaling factor.
    // This makes sure that the bark bands amplitudes will be scaled to be in comparison with each other.
    std::vector<unsigned int> scaling_factor;
    scaling_factor.resize(24);

    for (int i = 0; i < 24; ++i)
    {
        m_left_energy[i] = 0;
        m_right_energy[i] = 0;

        m_left_bark_max_amplitude[i] = 0.0f;
        m_right_bark_max_amplitude[i] = 0.0f;
    }

    float scale = m_spectrum_scale.value_at(time);

    for (int count = 0; count < spectrum_size; ++count)
    {
        // 23.4375 hz * 1024 = 24000 hz
        float lower_hz = m_entry_hz * (count);
        float higher_hz = m_entry_hz * (count + 1);
        float hz = lower_hz + m_entry_hz * 0.5f; // Average hz for this entry.
        size_t bark_index = this->bark_index(hz);

        // Apply A-weighting. Offset in decibels what human ear can hear for given hertz. This offset is mostly negative.
        //float decibel_m = decibel_offset(higher_hz);
        //float decibel_offset = decibel_m;

        // Adjust by A-weighting.
        //float temp_amplitude = std::pow(10, decibel_offset / 20.0f);
        float temp_amplitude = db_to_amplitude(decibel_offset(higher_hz));
        // dB gain is just multiply. See https://en.wikipedia.org/wiki/Decibel
        m_left_amplitude[count] *= temp_amplitude;
        m_right_amplitude[count] *= temp_amplitude;

        // Decibel for current hz.
        float left_decibel = amplitude_to_db(m_left_amplitude[count]);
        float right_decibel = amplitude_to_db(m_right_amplitude[count]);
        float min_decibel = -100.0f;
        float max_decibel = 0.0f;
        m_left_decibel[count] = parameterize(min_decibel, max_decibel, 0.0f, 1.0f, left_decibel);
        m_right_decibel[count] = parameterize(min_decibel, max_decibel, 0.0f, 1.0f, right_decibel);

        // Hack.
        m_left_amplitude[count] *= 7.5f;
        m_right_amplitude[count] *= 7.5f;

        // Scale amplitudes. Bark amplitudes and total energy will get scaled too.
        m_left_amplitude[count] *= scale;
        m_right_amplitude[count] *= scale;

        m_left_amplitude[count] = pt::clamp(m_left_amplitude[count], 0.0f, 1.0f);
        m_right_amplitude[count] = pt::clamp(m_right_amplitude[count], 0.0f, 1.0f);

        m_left_energy[bark_index] += m_left_amplitude[count];
        m_right_energy[bark_index] += m_right_amplitude[count];
        scaling_factor[bark_index]++; // count of entries per bark to calculate average.

        //// Find max amplitude for bark.
        //if (m_left_amplitude[count] > m_left_bark_max_amplitude[bark_index])
        //    m_left_bark_max_amplitude[bark_index] = m_left_amplitude[count];
        //if (m_right_amplitude[count] > m_right_bark_max_amplitude[bark_index])
        //    m_right_bark_max_amplitude[bark_index] = m_right_amplitude[count];
    }

    // Total energy for each bark.
    for (int i = 0; i < 24; ++i)
    {
        // Average bark energy by sample count in the bark.
        m_left_energy[i] /= scaling_factor[i];
        m_right_energy[i] /= scaling_factor[i];

        // Hacks. Reduce zero-line and increase scale.
        m_left_energy[i]  -= 0.005f;
        m_right_energy[i] -= 0.005f;
        m_left_energy[i]  *= 1.25f;
        m_right_energy[i] *= 1.25f;

        m_left_energy[i] = pt::clamp(m_left_energy[i], 0.0f, 1.0f);
        m_right_energy[i] = pt::clamp(m_right_energy[i], 0.0f, 1.0f);
    }

    // Total energy for left and right.
    m_total_energy[0] = 0.0f;
    m_total_energy[1] = 0.0f;
    for (int i = 0; i < 24; ++i)
    {
        m_total_energy[0] += m_left_energy[i];
        m_total_energy[1] += m_right_energy[i];
    }

    // Average of all bark amplitudes.
    m_total_energy[0] /= 24.0f;
    m_total_energy[1] /= 24.0f;
    // Hack. Remove very low noise and then amplify a bit.
    m_total_energy[0] -= 0.005f;
    m_total_energy[1] -= 0.005f;
    m_total_energy[0] *= 3.0f;
    m_total_energy[1] *= 3.0f;
    m_total_energy[0] = pt::clamp(m_total_energy[0], 0.0f, 1.0f);
    m_total_energy[1] = pt::clamp(m_total_energy[1], 0.0f, 1.0f);

    for (int count = 0; count < spectrum_size; ++count)
    {
        float hz = m_entry_hz * (count + 0);
        //float hz = m_entry_hz * (count + 1);
        //size_t bark_index = this->bark_index(hz);

        // m_left_energy contains the total energy for the bark. It should mark the upmost limit for this frequency.
        // m_left_spectrum and m_left_energy should be in range 0--1.
        //m_left_spectrum[count] *= m_left_energy[bark_index];
        //m_right_spectrum[count] *= m_right_energy[bark_index];


        //// Normalize the data by multiplying 1/max for each bark.
        //float inv_left_max = 1.0f / m_left_bark_max_amplitude[bark_index];
        //float inv_right_max = 1.0f / m_right_bark_max_amplitude[bark_index];
        //m_left_amplitude[count] *= inv_left_max;
        //m_right_amplitude[count] *= inv_right_max;

        // this means: the more amplitudes the band has, the more it will be scaled down
        //float tmp = ((float)total_scaling_factor / (float)(total_scaling_factor + scaling_factor[bark_index]));
        //m_left_spectrum[count] *= tmp;
        //m_right_spectrum[count] *= tmp;

        //m_left_bark_max_amplitude
        //m_left_spectrum[count] = (m_left_energy[bark_index] / static_cast<float>(scaling_factor[bark_index]));
        //m_right_spectrum[count] = (m_right_energy[bark_index] / static_cast<float>(scaling_factor[bark_index]));
        //m_left_spectrum[count] = (m_left_energy[bark_index] / static_cast<float>(scaling_factor[bark_index])) * 10;// *inv_left_max;
        //m_right_spectrum[count] = (m_right_energy[bark_index] / static_cast<float>(scaling_factor[bark_index])) * 10;// *inv_right_max;
    }
}

std::vector<float> const& audio::left_amplitude() const
{
    return m_left_amplitude;
}

std::vector<float> const& audio::right_amplitude() const
{
    return m_right_amplitude;
}

std::vector<float> const& audio::left_decibel() const
{
    return m_left_decibel;
}

std::vector<float> const& audio::right_decibel() const
{
    return m_right_decibel;
}

std::vector<float> audio::total_energy() const
{
    return m_total_energy;
}

//float audio::total_energy() const
//{
//    return m_total_energy;
//}

std::vector<float> audio::left_energy() const
{
    return m_left_energy;
}

std::vector<float> audio::right_energy() const
{
    return m_right_energy;
}

float audio::entry_hz() const
{
    return m_entry_hz;
}

size_t audio::bark_index(float hz) const
{
    // Hack, even barks. Hack more to round down.
    //float tmp = (hz - 0.001f) / (m_frequency / 2.0f);
    //size_t result = static_cast<size_t>(tmp * 24);
    //assert(result < 24);
    //return result;

    for (size_t i = 0; i < 24; ++i)
    {
        if (hz < bark_cut_off_frequency[i])
            return i;
    }
    return 23;
}

bool audio::is_playing() const
{
    return m_is_playing;
}

vector4f audio::vector4f_spectrum(float time) const
{
    std::shared_ptr<fmod_impl::system> system = m_system.lock();
    if (!system)
        return vector4f();

    int step0_count = 0;
    int step1_count = 0;
    int step2_count = 0;
    int step3_count = 0;

    float min_threshold = m_spectrum_min_threshold.value_at(time);
    float max_threshold = m_spectrum_max_threshold.value_at(time);

    vector4f result;
    //int step0 = 256;
    //int step1 = 512;
    //int step2 = 768;
    //int step3 = 1024;

    //int step0 = static_cast<int>(floor(69.30f / entry_hz)); // 3
    //int step1 = static_cast<int>(floor(415.30f / entry_hz)); // 19
    //int step2 = static_cast<int>(floor(3951.07f / entry_hz)); // 183
    //int step3 = static_cast<int>(floor(1024));

    //float lower_hz = entry_hz * (count);
    //float higher_hz = entry_hz * (count + 1);
    //float hz = higher_hz;
    //size_t bark_index = get_bark_index(hz);

    // Round down 48000 hz fit 1024.
    // 48khz - m_left_spectrum has 1024 values.
    int step0 = (        static_cast<int>(static_cast<float>(output_rate) / 24.0f * 2.0f)) / 1024;
    int step1 = (step0 + static_cast<int>(static_cast<float>(output_rate) / 24.0f * 4.0f)) / 1024;
    int step2 = (step1 + static_cast<int>(static_cast<float>(output_rate) / 24.0f * 8.0f)) / 1024;
    int step3 = (step2 + static_cast<int>(static_cast<float>(output_rate) / 24.0f * 10.0f)) / 1024;


    for (int count = 0; count < step0; count++)
    {
        // Left
        float scaled_value = m_left_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[0] += scaled_value;
            ++step0_count;
        }
        // Right
        scaled_value = m_right_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[0] += scaled_value;
            ++step0_count;
        }
    }
    for (int count = step0; count < step1; count++)
    {
        // Left
        float scaled_value = m_left_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[1] += scaled_value;
            ++step1_count;
        }
        // Right
        scaled_value = m_right_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[1] += scaled_value;
            ++step1_count;
        }
    }
    for (int count = step1; count < step2; count++)
    {
        // Left
        float scaled_value = m_left_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[2] += scaled_value;
            ++step2_count;
        }
        // Right
        scaled_value = m_right_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[2] += scaled_value;
            ++step2_count;
        }
    }
    for (int count = step2; count < step3; count++)
    {
        // Left
        float scaled_value = m_left_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[3] += scaled_value;
            ++step3_count;
        }
        // Right
        scaled_value = m_right_amplitude[count];
        if (scaled_value > min_threshold && scaled_value < max_threshold)
        {
            result[3] += scaled_value;
            ++step3_count;
        }
    }

    // Get averages
    if (step0_count > 0)
        result[0] = result[0] / step0_count;
    if (step1_count > 1)
        result[1] = result[1] / step1_count;
    if (step2_count > 2)
        result[2] = result[2] / step2_count;
    if (step3_count > 3)
        result[3] = result[3] / step3_count;

    result[0] = 1.0f - std::cosf(result[0] * 3.1415f / 2.0f);
    result[1] = 1.0f - std::cosf(result[1] * 3.1415f / 2.0f);
    result[2] = 1.0f - std::cosf(result[2] * 3.1415f / 2.0f);
    result[3] = 1.0f - std::cosf(result[3] * 3.1415f / 2.0f);
    result[0] = pt::clamp(result[0], 0.0f, 255.0f);
    result[1] = pt::clamp(result[1], 0.0f, 255.0f);
    result[2] = pt::clamp(result[2], 0.0f, 255.0f);
    result[3] = pt::clamp(result[3], 0.0f, 255.0f);


    //result[0] *= 2.0f;
    //result[1] *= 2.0f;
    //result[2] *= 2.0f;
    //result[3] *= 2.0f;

    return result;

    //int bin = 0;
    //float max = 0;
    //for (int count = 0; count < SPECTRUMSIZE; count++)
    //{
    //    if (spectrum[count] > 0.01f && spectrum[count] > max)
    //    {
    //        max = spectrum[count];
    //        bin = count;
    //    }
    //}

    //float dominanthz = (float)bin * BINSIZE; // dominant frequency min

    //return vector4f(max, max, max, max);
    ////return vector4f(spectrum[0], spectrum[1], spectrum[2], spectrum[3]);
}

std::shared_ptr<d3d9::texture_2d> audio::on_draw(rendertarget&,
    float time, std::shared_ptr<d3d9::texture_2d> child_result)
{
    if (!m_channel)
    {
        m_channel = m_sound->play();

        FMOD_RESULT res = m_channel->getFrequency(&m_frequency);
        if (res != FMOD_OK)
        {
            // Default to 44k but dont fail. Syncs might be off.
            m_frequency = 44100;
        }
    }

    if (std::shared_ptr<fmod_impl::system> system = m_system.lock())
    {
        int channels;
        fmod_impl::check_fmod(system->native()->getChannelsPlaying(&channels),
            "System::getChannelsPlaying");

        if (channels == 0)
            return child_result;
    }
    else
    {
        return child_result;
    }

    fmod_impl::check_fmod(m_channel->isPlaying(&m_is_playing),
        "FMOD::Channel::isPlaying");
    if (!m_is_playing)
        return child_result;

    bool is_paused;
    fmod_impl::check_fmod(m_channel->getPaused(&is_paused),
        "FMOD::Channel::getPaused");

    if (is_paused)
        m_channel->setPaused(false);

    if (time < m_time)
    {
        unsigned int position = static_cast<unsigned int>(time * 1000.0f);

        fmod_impl::check_fmod(m_channel->setPosition(position,
            FMOD_TIMEUNIT_MS), "FMOD::Channel::setPosition");
    }

    m_time = time;

    return child_result;
}

}} // namespace content::sequences
