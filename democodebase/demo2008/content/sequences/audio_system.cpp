#include "audio_system.h"
#include "audio.h"
#include "pt/config_node.h"
#include "pt/parameter_collection.h"
#include "external/fmod_impl/system.h"
#include "../user_abort.h"
#include "../utility.h"
#include "pt/string_wrapper.h"

namespace content { namespace sequences {

audio_system::audio_system(config_node const& config, parameter_collection const& parameters)
: simple_composite_sequence(config)
, m_system(new fmod_impl::system)
{
    assert(config.key() == "audio_system");

    for (config_node::const_iterator i = config.begin(); i != config.end();
        ++i)
    {
        if (i->key() == "audio_sequence")
        {
            std::string path = i->child("path")->value();

            //float spectrum_scale = 1.0f;
            //float spectrum_min_threshold = 0.0f;
            //float spectrum_max_threshold = 1.0f;
            std::shared_ptr<animatable_value<float> const> tmp_scale;
            std::shared_ptr<animatable_value<float> const> tmp_min_threshold;
            std::shared_ptr<animatable_value<float> const> tmp_max_threshold;

            auto spectrum_iterator = i->find_child("spectrum");
            if (spectrum_iterator != i->end())
            {
                std::shared_ptr<parameter_node> curves
                    = optional_child_node("curves", parameters);

                // scale = "2" min_threshold = "0.2" max_threshold = "1.0"
                tmp_scale = create_optional_curve_value<float>(*spectrum_iterator, "scale", curves, 1.0f);
                tmp_min_threshold = create_optional_curve_value<float>(*spectrum_iterator, "min_threshold", curves, 0.0f);
                tmp_max_threshold = create_optional_curve_value<float>(*spectrum_iterator, "max_threshold", curves, 1.0f);
            }
            else
            {
                tmp_scale = create_constant_value(1.0f);
                tmp_min_threshold = create_constant_value(0.0f);
                tmp_max_threshold = create_constant_value(1.0f);
            }

            animatable_value<float> spectrum_scale = *tmp_scale;
            animatable_value<float> spectrum_min_threshold = *tmp_min_threshold;
            animatable_value<float> spectrum_max_threshold = *tmp_max_threshold;

            m_audio.reset(new audio(m_system, path, *i, spectrum_scale, spectrum_min_threshold, spectrum_max_threshold));
            //std::shared_ptr<composite_sequence> child(new audio(m_system, path, *i));

            add_child(m_audio, "audio_sequence");
        }
    }
}

audio& audio_system::get_audio()
{
    return *m_audio;
}

std::shared_ptr<d3d9::texture_2d> audio_system::on_draw(rendertarget&,
    float, std::shared_ptr<d3d9::texture_2d> child_result)
{
    m_system->update();

    //HACK
    int channels;
    fmod_impl::check_fmod(m_system->native()->getChannelsPlaying(&channels),
        "System::getChannelsPlaying");

    if (channels == 0)
        throw user_abort();

    return child_result;
}

}} // namespace content::sequences
