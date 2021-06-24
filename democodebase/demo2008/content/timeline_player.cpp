#include "timeline_player.h"
#include "timeline.h"
#include "sequences/audio_system.h"
#include "sequences/audio.h"
#include "sequences/composite_sequence.h"
#include "sequences/rendertarget.h"
#include "pt/animatable_value.h"
#include "pt/basic_value_source.h"
#include "pt/parameter_node.h"
#include "pt/parameter_node_utility.h"
#include "pt/parameter_collection.h"
#include "pt/vector.h"
#include "pt/exceptions/any_error.h"
#include "pt/d3d9/rendering/material_cache.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/std/cassert.h"


namespace content {

timeline_player::timeline_player(d3d9::graphics_device& device,
    path const& path, d3d9::rendering::material_cache& materials,
    parameter_collection const& parameters,
    progress_report* progress,
    bool only_serialize)
: m_device(device)
, m_view(0)
{
    // Replace audio_spectrum_segment1 source.
    {
        auto iterator = materials.shared_parameters().child("audio_spectrum_segment_1");
        std::shared_ptr<parameter_node> node = *iterator;

        m_audio_spectrum_segment_1 = std::make_shared<basic_value_source<float>>(0.0f);
        auto value = std::make_shared<animatable_value<float>>(m_audio_spectrum_segment_1);
        node->set_value(value);
    }
    // Replace audio_spectrum_segment2 source.
    {
        auto iterator = materials.shared_parameters().child("audio_spectrum_segment_2");
        std::shared_ptr<parameter_node> node = *iterator;

        m_audio_spectrum_segment_2 = std::make_shared<basic_value_source<float>>(0.0f);
        auto value = std::make_shared<animatable_value<float>>(m_audio_spectrum_segment_2);
        node->set_value(value);
    }
    // Replace audio_spectrum_segment3 source.
    {
        auto iterator = materials.shared_parameters().child("audio_spectrum_segment_3");
        std::shared_ptr<parameter_node> node = *iterator;

        m_audio_spectrum_segment_3 = std::make_shared<basic_value_source<float>>(0.0f);
        auto value = std::make_shared<animatable_value<float>>(m_audio_spectrum_segment_3);
        node->set_value(value);
    }
    // Replace audio_spectrum_segment4 source.
    {
        auto iterator = materials.shared_parameters().child("audio_spectrum_segment_4");
        std::shared_ptr<parameter_node> node = *iterator;

        m_audio_spectrum_segment_4 = std::make_shared<basic_value_source<float>>(0.0f);
        auto value = std::make_shared<animatable_value<float>>(m_audio_spectrum_segment_4);
        node->set_value(value);
    }

    // Replace audio_spectrum_vector4f source.
    {
        auto iterator = materials.shared_parameters().child("audio_spectrum_vector4f");
        std::shared_ptr<parameter_node> node = *iterator;

        m_audio_spectrum_source = std::make_shared<basic_value_source<vector4f>>(vector4f());
        auto value = std::make_shared<animatable_value<vector4f>>(m_audio_spectrum_source);
        node->set_value(value);
    }

    //
    m_timeline.reset(new timeline(device, path, materials, parameters, progress, only_serialize));
    assert(m_timeline);
}

//timeline_player::timeline_player(d3d9::graphics_device& device,
//	std::shared_ptr<timeline> timeline)
//: m_device(device)
//, m_timeline(timeline)
//, m_view(0)
//{
//    assert(m_timeline);
//}

void timeline_player::draw(float time)
{
    if (m_timeline->audio_system_sequence())
    {
        std::shared_ptr<content::sequences::audio_system> audio_system = m_timeline->audio_system_sequence();

        audio_system->get_audio().update_spectrum(time);

        // TODO: Clear to black if not playing
        if (audio_system->get_audio().is_playing())
        {
            // Create textures containing the audio spectrum data
            auto lockable_512_texture = d3d9::default_texture_2d_collection().find("Lockable512");
            auto lockable_1008_256_texture = d3d9::default_texture_2d_collection().find("Lockable1008_256");
            //auto lockable_16_texture = d3d9::default_texture_2d_collection().find("Lockable16");
            auto lockable_24_16_texture = d3d9::default_texture_2d_collection().find("Lockable24_16");
            auto lockable_24_1_texture = d3d9::default_texture_2d_collection().find("Lockable24_1");
            auto lockable_16_1_texture = d3d9::default_texture_2d_collection().find("Lockable16_1");

            auto audio_amplitudes_texture = d3d9::default_texture_2d_collection().find("AudioAmplitudes");
            auto audio_bark_amplitudes_texture = d3d9::default_texture_2d_collection().find("AudioBarkAmplitudes");
            auto audio_bark_amplitudes_hd_texture = d3d9::default_texture_2d_collection().find("AudioBarkAmplitudesHD");
            auto audio_bark_amplitudes_scalar_texture = d3d9::default_texture_2d_collection().find("AudioBarkAmplitudes_scalar");
            auto audio_total_energy_texture = d3d9::default_texture_2d_collection().find("AudioTotalEnergy");

            //vector2f amplitude_resolution = { 512.0f, 512.0f };
            //vector2f bark_amplitude_resolution = { 16.0f, 16.0f };
            //vector2i total_energy_resolution = { 16, 1 };
            vector2i amplitude_resolution = audio_amplitudes_texture->resolution();
            vector2i bark_amplitude_resolution = audio_bark_amplitudes_texture->resolution();
            vector2i bark_amplitudes_16_1_resolution = audio_total_energy_texture->resolution();
            vector2i total_energy_resolution = audio_total_energy_texture->resolution();

            if (!lockable_512_texture || !lockable_1008_256_texture || !lockable_24_16_texture || !lockable_24_1_texture || !lockable_16_1_texture)
                throw pt::exceptions::any_error("Shared texture is missing some lockable textures.");

            if (!audio_amplitudes_texture || !audio_bark_amplitudes_texture || !audio_bark_amplitudes_scalar_texture || !audio_total_energy_texture)
                throw pt::exceptions::any_error("Shared texture is missing some audio textures.");

            auto const& audio = audio_system->get_audio();

            // Total energy is in range [0, 1].
            //unsigned char total_energy = static_cast<unsigned char>(audio_system->get_audio().total_energy() * 0xff);

            // Fill amplitude. Source is [0, 1024) where 1023 is 24000hz. We will write directly to texture, using only first 512 (12000 hz).
            // TODO: Hardcoded, texture must be 512x512.
            {
                auto left_amplitude = audio.left_amplitude();
                auto right_amplitude = audio.right_amplitude();

                // Locked texture scope.
                {
                    d3d9::scoped_texture_2d_lock texture_512_lock(lockable_512_texture->native());
                    struct amplitude_data_t
                    {
                        uint32_t tex[512][512];
                    };
                    amplitude_data_t* amplitude_data = static_cast<amplitude_data_t*>(texture_512_lock.data());

                    for (int v = 0; v < amplitude_resolution[1]; ++v)
                    {
                        int target_v = (amplitude_resolution[1] - 1) - v;

                        for (int u = 0; u < amplitude_resolution[0]; ++u)
                        {
                            // U = Target hertz entry. Aim is to have range [0, 3600] hz in texture.
                            const int target_u = u;

                            // u = [0, 512] = [0, 24000] hz. We want source_v to be [0, 3600] hz.
                            //int source_u = (u / 512.0f) * (24000.0f / 3600.0f);

                            // Divide U by (1024 / 6.67) to use only 3600 hz, so it matches the barks. Entry 1024 is 24000 hz.
                            const size_t source_entry_index = u;// static_cast<size_t>((u * 2.0f) / 6.67f);

                            float hz = u * audio.entry_hz();
                            size_t bark_index = audio.bark_index(hz);
                            unsigned char background = bark_index;
                            if (bark_index > 23)
                                background = 0;
                            if ((bark_index % 2) == 0)
                                background = 0;
                            background *= 10;

                            const bool is_left_lit = left_amplitude[source_entry_index] * amplitude_resolution[0] > v;
                            const bool is_right_lit = right_amplitude[source_entry_index] * amplitude_resolution[0] > v;
                            //const unsigned char background = 0;// total_energy / 4;

                            const int r_int = is_left_lit ? 0xff : 0;
                            const int g_int = is_right_lit ? 0xff : 0;
                            const int b_int = background; // right ? 0xff : 0;

                            unsigned char r = static_cast<unsigned char>(r_int);
                            unsigned char g = static_cast<unsigned char>(g_int);
                            unsigned char b = static_cast<unsigned char>(b_int);
                            const int a = 0xff;

                            amplitude_data->tex[target_v][target_u] = r + (g << 8) + (b << 16) + (a << 24);
                        }
                    }
                }
                update_texture(m_device, lockable_512_texture->native(), audio_amplitudes_texture->native());
            }

            // Fill bark amplitudes. Source is [0, 24) to fill 24x16 texture.
            {
                auto left_energy = audio_system->get_audio().left_energy();
                auto right_energy = audio_system->get_audio().right_energy();

                // Locked 24 x 16 texture scope.
                {
                    struct bark_amplitude_data_t
                    {
                        uint32_t tex[16][24];
                    };

                    d3d9::scoped_texture_2d_lock texture_24_16_lock(lockable_24_16_texture->native());
                    bark_amplitude_data_t* bark_amplitude_data = static_cast<bark_amplitude_data_t*>(texture_24_16_lock.data());

                    for (int v = 0; v < bark_amplitude_resolution[1]; ++v)
                    {
                        int target_v = (bark_amplitude_resolution[1] - 1) - v;

                        for (int u = 0; u < bark_amplitude_resolution[0]; ++u)
                        {
                            // U = bark index.
                            size_t bark_index = u;
                            auto energy_left = left_energy[bark_index];
                            auto energy_right = right_energy[bark_index];

                            int target_u = u;

                            ////float hz = u * audio.entry_hz();
                            ////size_t bark_index = audio.bark_index(hz);
                            //unsigned char background = bark_index;
                            //if (bark_index > 23)
                            //    background = 0;
                            //if ((bark_index % 2) == 0)
                            //    background = 0;
                            //background *= 10;

                            const bool is_left_lit = energy_left * bark_amplitude_resolution[0] > v;
                            const bool is_right_lit = energy_right * bark_amplitude_resolution[0] > v;
                            unsigned char background = 0;// total_energy / 4;

                            const int r_int = (is_left_lit ? 0xff : 0);
                            const int g_int = (is_right_lit ? 0xff : 0);
                            const int b_int = background; // right ? 0xff : 0;

                            unsigned char r = static_cast<unsigned char>(r_int);
                            unsigned char g = static_cast<unsigned char>(g_int);
                            unsigned char b = static_cast<unsigned char>(b_int);
                            const int a = 0xff;

                            bark_amplitude_data->tex[target_v][target_u] = r + (g << 8) + (b << 16) + (a << 24);

                        }
                    }
                    update_texture(m_device, lockable_24_16_texture->native(), audio_bark_amplitudes_texture->native());
                }

                // Locked 1008 x 256 texture scope.
                {
                    const int target_resolution_u = 1008;
                    const int target_resolution_v = 256;

                    struct bark_amplitude_hd_data_t
                    {
                        uint32_t tex[target_resolution_v][target_resolution_u];
                    };

                    d3d9::scoped_texture_2d_lock texture_1008_256_lock(lockable_1008_256_texture->native());
                    bark_amplitude_hd_data_t* bark_amplitude_hd_data = static_cast<bark_amplitude_hd_data_t*>(texture_1008_256_lock.data());

                    for (int v = 0; v < target_resolution_v; ++v)
                    {
                        int target_v = (target_resolution_v - 1) - v;

                        for (int u = 0; u < target_resolution_u; ++u)
                        {
                            // U = bark index. Bark size is 42 pixels.
                            size_t bark_index = static_cast<size_t>((u / static_cast<float>(target_resolution_u)) * 24);
                            int bark_u = u - (bark_index * 42);

                            auto energy_left = left_energy[bark_index];
                            auto energy_right = right_energy[bark_index];

                            ////float hz = u * audio.entry_hz();
                            ////size_t bark_index = audio.bark_index(hz);
                            //unsigned char background = bark_index;
                            //if (bark_index > 23)
                            //    background = 0;
                            //if ((bark_index % 2) == 0)
                            //    background = 0;
                            //background *= 10;
                            const bool is_border = bark_u == 0 || bark_u == 41;

                            //unsigned char background = bark_u / 42;// total_energy / 4;
                            unsigned char background = (is_border ? 0x7f : 0);
                            //unsigned char background = 255.0f * (bark_u / 42.0f);// total_energy / 4;

                            int r_int = 0;
                            int g_int = 0;
                            int b_int = 0;

                            bool is_lit = false;
                            float average_energy = pt::average(energy_left, energy_right);
                            float v_range = v / static_cast<float>(target_resolution_v); // [0, 1] with respect of v
                            float energy_range_256 = (average_energy * 256.0f) / static_cast<float>(target_resolution_v);
                            is_lit = average_energy > v_range;

                            float asdasd = 1.0f - ((average_energy - v_range) / average_energy);
                            asdasd = asdasd * asdasd * asdasd;
                            float tmp4 = pt::clamp(asdasd * 255.0f, 0.0f, 255.0f);

                            if (is_border)
                            {
                                if (is_lit)
                                {
                                    r_int = tmp4 / 2;
                                    g_int = tmp4 / 2;
                                    b_int = tmp4 / 2;
                                }
                            }
                            else
                            {
                                int shade = 0;
                                if (is_lit)
                                {
                                    //float tmp4 = pt::clamp(average_energy / v_range, 0.0f, 1.0f);
                                    //float tmp3 = pt::clamp(static_cast<int>(tmp4 * 255.0f), 0, 255);
                                    shade = static_cast<int>(tmp4);
                                }

                                r_int = shade;
                                g_int = shade;
                                b_int = shade;
                            }

                            unsigned char r = static_cast<unsigned char>(r_int);
                            unsigned char g = static_cast<unsigned char>(g_int);
                            unsigned char b = static_cast<unsigned char>(b_int);
                            const int a = 0xff;

                            int target_u = u;
                            bark_amplitude_hd_data->tex[target_v][target_u] = r + (g << 8) + (b << 16) + (a << 24);

                        }
                    }
                    update_texture(m_device, lockable_1008_256_texture->native(), audio_bark_amplitudes_hd_texture->native());
                }

                // Locked texture scope.
                {
                    d3d9::scoped_texture_2d_lock texture_24_1_bark_lock(lockable_24_1_texture->native());
                    uint32_t* bark_amplitudes = static_cast<uint32_t*>(texture_24_1_bark_lock.data());

                    for (int v = 0; v < bark_amplitude_resolution[0]; ++v)
                    {
                        size_t bark_index = v;
                        float energy_left = left_energy[bark_index];
                        float energy_right = right_energy[bark_index];

                        // Hack. Scale up.
                        energy_left  = pt::clamp(energy_left  * 1.3f, 0.0f, 1.0f);
                        energy_right = pt::clamp(energy_right * 1.3f, 0.0f, 1.0f);

                        unsigned char r = static_cast<unsigned char>(energy_left * 255);
                        unsigned char g = static_cast<unsigned char>(energy_right * 255);
                        unsigned char b = static_cast<unsigned char>((r + g) / 2);
                        const unsigned char a = 0xff;
                        bark_amplitudes[v] = r + (g << 8) + (b << 16) + (a << 24);
                    }
                }
                update_texture(m_device, lockable_24_1_texture->native(), audio_bark_amplitudes_scalar_texture->native());
            }

            // Fill total energy. Texture is 16 x 1.
            {
                struct total_energy_data_t
                {
                    uint32_t tex[16][1];
                };

                // Left and right total energy.
                std::vector<float> total_energy = audio_system->get_audio().total_energy();
                d3d9::scoped_texture_2d_lock texture_16_1_lock(lockable_16_1_texture->native());
                total_energy_data_t* total_energy_data = static_cast<total_energy_data_t*>(texture_16_1_lock.data());
                for (int v = 0; v < total_energy_resolution[1]; ++v)
                {
                    for (int u = 0; u < total_energy_resolution[0]; ++u)
                    {
                        auto energy_left = total_energy[0];
                        auto energy_right = total_energy[1];

                        const bool is_left_lit = energy_left * total_energy_resolution[0] > u;
                        const bool is_right_lit = energy_right * total_energy_resolution[0] > u;

                        const int r_int = is_left_lit ? 0xff : 0;
                        const int g_int = is_right_lit ? 0xff : 0;

                        //float hack_coeff = 1.0f;
                        //float background_scale_01 = std::max(((energy_left + energy_right) / 2.0f) * hack_coeff, 1.0f);
                        float background_scale_01 = std::min(((energy_left + energy_right) / 2.0f), 1.0f);
                        float background_scale_coeff = 1.0f - std::cosf(background_scale_01 * 3.1415f / 2.0f);
                        int background = background_scale_coeff * 255.0f;
                        background = pt::clamp(background, 0, 255);
                        const int b_int = background;


                        //// Scale from [-40, 0] to [0, 255]
                        //float decibel = content::sequences::amplitude_to_db((energy_left + energy_right) / 2.0f);
                        //int background = pt::parameterize(-40.0f, 0.0f, 0.0f, 40.0f, decibel);
                        //background = pt::clamp(background, 0, 255);
                        //const int b_int = background;

                        //int background = static_cast<int>(((energy_left + energy_right) / 2.0f) * 255.0f);
                        //background = pt::clamp(background, 0, 255);
                        //const int b_int = background;

                        unsigned char r = static_cast<unsigned char>(r_int);
                        unsigned char g = static_cast<unsigned char>(g_int);
                        unsigned char b = static_cast<unsigned char>(b_int);
                        const int a = 0xff;

                        total_energy_data->tex[u][v] = r + (g << 8) + (b << 16) + (a << 24);

                    }
                }

                update_texture(m_device, lockable_16_1_texture->native(), audio_total_energy_texture->native());
            }
        }

        vector4f tmp = audio_system->get_audio().vector4f_spectrum(time);
        m_audio_spectrum_source->set(tmp);

        m_audio_spectrum_segment_1->set(tmp[0]);
        m_audio_spectrum_segment_2->set(tmp[1]);
        m_audio_spectrum_segment_3->set(tmp[2]);
        m_audio_spectrum_segment_4->set(tmp[3]);
    }

    m_active_sequences = m_timeline->active_sequences_at(time);

    content::sequences::rendertarget null_target;

    for (sequences::const_iterator i = m_active_sequences.begin();
        i != m_active_sequences.end(); ++i)
    {
        // Update
        // TODO take fps into calculation.
        float sequence_local_time = time - (*i)->start_time();

        (*i)->set_override_view(m_view);

        // Draw
        (*i)->set_override_view(m_view);
        (*i)->draw(null_target, sequence_local_time);
    }
}

void timeline_player::set_view(d3d9::rendering::item* view)
{
    m_view = view;
}

} // namespace content
