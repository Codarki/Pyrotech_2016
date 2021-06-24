#include "audio_sync.h"
#include "external/fmod_impl/system.h"
#include <limits>

namespace content { namespace sequences {

audio_sync::audio_sync(std::shared_ptr<fmod_impl::system> fmod_system, int sampling_rate, int num_fourier_values)
: m_fmod_system(fmod_system)
{   
    m_sampling_rate      = sampling_rate;
    m_num_fourier_values = num_fourier_values;
    m_step_hz            = static_cast<float>(sampling_rate / 2) / num_fourier_values;

    m_amplitudes_left    = shared_array<float>(new float[m_num_fourier_values]);
    m_amplitudes_right   = shared_array<float>(new float[m_num_fourier_values]);
  
}

float audio_sync::calc_spectrum()
{
    float sum_spectrum = 0;

    fmod_impl::check_fmod(
        m_fmod_system->native()->getSpectrum(m_amplitudes_left.get(),
        m_num_fourier_values, 0, FMOD_DSP_FFT_WINDOW_BLACKMAN), "FMOD::System::getSpectrum" // 0 == LEFT CHANNEL OF STEREO
       );

    fmod_impl::check_fmod(
        m_fmod_system->native()->getSpectrum(m_amplitudes_right.get(),
        m_num_fourier_values, 1, FMOD_DSP_FFT_WINDOW_BLACKMAN), "FMOD::System::getSpectrum" // 1 == RIGHT CHANNEL OF STEREO
       );

    // calculate the energy of the band
    int starting_index = 0;
    float temp_freq = 0;
    float FREQ1 = 0;
    float FREQ2 = 0;
    float temp_add = 0;
    unsigned int scaling_factor[24]; // each bark band has own amplitude scaling factor

    for (int i = 0; i < 24; i++)
    {
        scaling_factor[i] = 0;
    }

    // CALCULATE ENERGY FOR EACH BARK SCALE FREQUENCY BAND(24 FREQUENCY BANDS) ///////////////////////////////////////////////
    for (int b = 0; b < 24; b++)
    {

        // set the borders. FREQ1 is the lower border, FREQ2 is the higher one.
        FREQ1 = m_barkFreqs[b]; // m_barkFreqs is a table with bark values.
        FREQ2 = m_barkFreqs[b+1];

        for (int i = 0; i < m_num_fourier_values; i++)
        {
            // search for the starting index with this frequency area
            if (temp_freq >= FREQ1)
            {
                starting_index = i;
                break;
            }
            temp_freq += m_step_hz; // didn't find the starting index ->step forward
        }

        // fill with left channel amplitudes
        int j=0;
        for (float i = FREQ1; i < FREQ2; i+=m_step_hz)
        {
            temp_add = fabs(m_amplitudes_left[starting_index+j]);

            if (temp_add)
            {
                m_energies[b] += temp_add;
                scaling_factor[b]++; // this makes sure that the bark bands amplitudes will be scaled to be in comparison with each other
            }
            j++;
        }

        // fill with right channel amplitudes
        j = 0;
        for (float i = FREQ1; i < FREQ2; i+=m_step_hz)
        {
            temp_add = fabs(m_amplitudes_right[starting_index+j]);

            if (temp_add)
            {
                m_energies[b] += temp_add;
                scaling_factor[b]++; // this makes sure that the bark bands amplitudes will be scaled to be in comparison with each other
            }
            j++;
        }

        m_energies[b] /= 2; // average of right and left channel

    } // end for (int b = 0; b < 24; b++)

    unsigned int total_scaling_factor = 0;
    for (int i = 0; i < 24; i++)
    {
        total_scaling_factor += scaling_factor[i];
    }
    for (int i = 0; i < 24; i++)
    {
        m_energies[i] *= ((float)total_scaling_factor / (float)(total_scaling_factor+scaling_factor[i])); // this means: the more amplitudes the band has, the more it will be scaled down
    }

    // calculate total energy
    m_totalEnergy = GetEnergies(0, 23);    

    return sum_spectrum;
}

}}
