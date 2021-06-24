#include "post_processing_parameters.h"
#include "bloom_parameters.h"
#include "dof_parameters.h"
#include "light_scattering_parameters.h"
#include "../utility.h"
#include "pt/ui/common.h"
#include "pt/ui/input_event.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/overridable_value_source.h"

namespace content { namespace sequences {

// post_processing_parameters

namespace {
} // anonymous namespace

post_processing_parameters::post_processing_parameters(
    config_node const& config, std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("post_processing_parameters")
,   m_time(0.0f)
{
    if (config.key() != "post_processing_parameters")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as post_processing_parameters");
    }

    config_node parameters(*config.child("parameters"));

    m_bloom.reset(new bloom_parameters(*parameters.child("bloom"), curves));
    m_tone_mapping.reset(new tone_mapping_parameters(
        *parameters.child("tone_mapping"), curves));

    m_noise.reset(new noise_parameters(*parameters.child("noise"), curves));

    m_dof.reset(new dof_parameters(*parameters.child("depth_of_field"), curves));
	m_light_scattering.reset(new light_scattering_parameters(*parameters.child("light_scattering"), curves));

    add(m_bloom->parameter());
    add(m_tone_mapping->parameter());
    add(m_noise->parameter());
    add(m_dof->parameter());
	add(m_light_scattering->parameter());

    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "Post processing parameters:", nullptr, vector4f(1,1,1,1)));
    add_text_sources(m_bloom->text_sources());
    add_text_sources(m_tone_mapping->text_sources());
    add_text_sources(m_noise->text_sources());
    add_text_sources(m_dof->text_sources());
	add_text_sources(m_light_scattering->text_sources());
}

void post_processing_parameters::on_set_current_time(float time)
{
    m_bloom->set_current_time(time);
    m_tone_mapping->set_current_time(time);
    m_noise->set_current_time(time);
    m_dof->set_current_time(time);
	m_light_scattering->set_current_time(time);
}

void post_processing_parameters::on_handle_key_down(ui::key_down_event& event)
{
    m_bloom->handle_key_down(event);
    m_tone_mapping->handle_key_down(event);
    m_noise->handle_key_down(event);
    m_dof->handle_key_down(event);
	m_light_scattering->handle_key_down(event);
}

void post_processing_parameters::on_clear_overrides()
{
    m_bloom->clear_overrides();
    m_tone_mapping->clear_overrides();
    m_noise->clear_overrides();
    m_dof->clear_overrides();
	m_light_scattering->clear_overrides();
}

// noise_parameters

noise_parameters::noise_parameters(config_node const& config,
    std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("noise")
{
    if (config.key() != "noise")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as noise");
    }

    // Create sources for parameter_node values.
    m_noise_strength = create_overridable_value_source<float>(
        *config.child("strength"), curves);
    m_noise_speed = create_overridable_value_source<float>(
        *config.child("speed"), curves);
    m_noise_frequency = create_overridable_value_source<float>(
        *config.child("frequency"), curves);

    add(create_basic_node("strength", m_noise_strength));
    add(create_basic_node("speed", m_noise_speed));
    add(create_basic_node("frequency", m_noise_frequency));

    // Create info text.

    m_info_text.reset(new d3d9::formatted_text_source<float,2>());
    m_info_text->set(boost::format(
        "    Noise strength %1%\n"
        "    Noise speed %2%\n"));
    m_info_text->set(0, 0.1f);
    m_info_text->set(1, 1.0f);
    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "Noise parameters:", nullptr, vector4f(1,1,1,1)));
    add_text_source(m_info_text);
}

void noise_parameters::on_set_current_time(float time)
{
    m_info_text->set(0, m_noise_strength->value_at(time));
    m_info_text->set(1, m_noise_speed->value_at(time));
  
}

void noise_parameters::on_handle_key_down(ui::key_down_event& event)
{
    //Noise strength N+ N-
    if (event.key_code() == VK_ADD && ui::is_key_down('N'))
        add_5_percent(*m_noise_strength, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('N') )
        dec_5_percent(*m_noise_strength, time());

    if (ui::is_key_down('S') && !ui::is_key_down('T'))
    {
        if (event.key_code() == VK_ADD)
            add_5_percent(*m_noise_speed, time());
        else if (event.key_code() == VK_SUBTRACT)
            dec_5_percent(*m_noise_speed, time());
    }

    m_info_text->set(0, m_noise_strength->value_at(time()));
    m_info_text->set(1, m_noise_speed->value_at(time()));
}

void noise_parameters::on_clear_overrides()
{
    m_noise_strength->set_value_source(nullptr);
    m_noise_speed->set_value_source(nullptr);
}

// tone_mapping_parameters

tone_mapping_parameters::tone_mapping_parameters(config_node const& config,
    std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("tone_mapping")
{
    if (config.key() != "tone_mapping")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as tone_mapping");
    }

    // Create sources for parameter_node values.

    m_adapted_luminance = create_overridable_value_source<float>(
        *config.child("adapted_luminance"), curves);
    m_middle_gray = create_overridable_value_source<float>(
        *config.child("middle_gray"), curves);
    m_saturation = create_overridable_value_source<float>(
        *config.child("saturation"), curves);
    m_white_point = create_overridable_value_source<float>(
        *config.child("white_point"), curves);
    m_color_correction = create_overridable_value_source<vector3f>(
        *config.child("color_correction"), curves);

    m_negation_color = create_overridable_value_source<vector3f>(
        *config.child("negation_color"), curves);

    m_attribute1 = create_overridable_value_source<vector4f>(
        *config.child("attribute1"), curves);
    m_attribute2 = create_overridable_value_source<vector4f>(
        *config.child("attribute2"), curves);
    m_attribute3 = create_overridable_value_source<vector4f>(
        *config.child("attribute3"), curves);
    m_attribute4 = create_overridable_value_source<vector4f>(
        *config.child("attribute4"), curves);
    m_attribute5 = create_overridable_value_source<vector4f>(
        *config.child("attribute5"), curves);

    add(create_basic_node("adapted_luminance", m_adapted_luminance));
    add(create_basic_node("middle_gray", m_middle_gray));
    add(create_basic_node("saturation", m_saturation));
    add(create_basic_node("white_point", m_white_point));
    add(create_basic_node("color_correction", m_color_correction));

    add(create_basic_node("negation_color", m_negation_color));

    add(create_basic_node("attribute1", m_attribute1));
    add(create_basic_node("attribute2", m_attribute2));
    add(create_basic_node("attribute3", m_attribute3));
    add(create_basic_node("attribute4", m_attribute4));
    add(create_basic_node("attribute5", m_attribute5));

    // Create info text.

    m_info_text.reset(new d3d9::formatted_text_source<float,4>());
    m_info_text->set(boost::format(
        "    Adapted luminance: %1%\n"
        "    Middle gray: %2%\n"
        "    Saturation: %3%\n"
        "    White point: %4%\n"));
    m_info_text->set(0, 1.0f);
    m_info_text->set(1, 1.0f);
    m_info_text->set(2, 1.0f);
    m_info_text->set(3, 1.0f);

    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "Tone mapping parameters:", nullptr, vector4f(1,1,1,1)));
    add_text_source(m_info_text);
}

void tone_mapping_parameters::on_set_current_time(float time)
{
    m_info_text->set(0, m_adapted_luminance->value_at(time));
    m_info_text->set(1, m_middle_gray->value_at(time));
    m_info_text->set(2, m_saturation->value_at(time));
    m_info_text->set(3, m_white_point->value_at(time));
}

void tone_mapping_parameters::on_handle_key_down(ui::key_down_event& event)
{
    // Adapted luminance T+ T-
    if (event.key_code() == VK_ADD && ui::is_key_down('Y'))
        add_5_percent(*m_adapted_luminance, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('Y'))
        dec_5_percent(*m_adapted_luminance, time());

    // Saturation TS+ TS-
    if (ui::is_key_down('T') && ui::is_key_down('S'))
    {
        if (event.key_code() == VK_ADD)
        {
            add_5_percent(*m_saturation, time());
            clamp_01(*m_saturation, time());
        }
        else if (event.key_code() == VK_SUBTRACT)
        {
            dec_5_percent(*m_saturation, time());
            clamp_01(*m_saturation, time());
        }
    }

    // White point W+ W-
    if (event.key_code() == VK_ADD && ui::is_key_down('W') && !ui::is_key_down('B'))
        add_5_percent(*m_white_point, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('W') && !ui::is_key_down('B'))
        dec_5_percent(*m_white_point, time());

    // Middle gray G+ G-
    //if (event.key_code() == VK_ADD && ui::is_key_down('G'))
    //    add_5_percent(*m_middle_gray, time());
    //else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('G'))
    //    dec_5_percent(*m_middle_gray, time());

    m_info_text->set(0, m_adapted_luminance->value_at(time()));
    m_info_text->set(1, m_middle_gray->value_at(time()));
    m_info_text->set(2, m_saturation->value_at(time()));
    m_info_text->set(3, m_white_point->value_at(time()));
}

void tone_mapping_parameters::on_clear_overrides()
{
    m_adapted_luminance->set_value_source(nullptr);
    m_middle_gray->set_value_source(nullptr);
    m_saturation->set_value_source(nullptr);
    m_white_point->set_value_source(nullptr);
}

}} // namespace content::sequences
