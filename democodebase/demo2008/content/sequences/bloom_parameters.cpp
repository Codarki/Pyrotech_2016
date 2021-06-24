#include "bloom_parameters.h"
#include "bright_pass_parameters.h"
#include "../utility.h"
#include "pt/ui/common.h"
#include "pt/ui/input_event.h"
//#include "pt/d3d9/texture_2d_data_set_builder.h"
//#include "pt/math/step_curve.h"
#include "pt/any_error.h"
#include "pt/config_node.h"

namespace content { namespace sequences {

// bloom_parameters

bloom_parameters::bloom_parameters(config_node const& config,
    std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("bloom")
{
    if (config.key() != "bloom")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as bloom");
    }

    // Create sources for parameter_node values.

    m_deviation = create_overridable_value_source<float>(
        *config.child("deviation"), curves);
    m_multiplier = create_overridable_value_source<float>(
        *config.child("multiplier"), curves);

    m_blur_strength = create_animatable_value_source<std::string>(
        *config.child("blur_strength_texture"), curves);
	m_horizontal_strength = create_animatable_value_source<std::string>(
		*config.child("horizontal_strength_texture"), curves);

    // Bloom input's bright pass
    m_bright_pass.reset(new bright_pass_parameters(
        *config.child("bright_pass"), curves));

    add(create_basic_node("deviation", m_deviation));
    add(create_basic_node("multiplier", m_multiplier));
    add(create_basic_node("blur_strength_texture", m_blur_strength));
	add(create_basic_node("horizontal_strength_texture", m_horizontal_strength));
    add(m_bright_pass->parameter());

    // Create info text.

    m_info_text.reset(new d3d9::formatted_text_source<float,2>());
    m_info_text->set(boost::format(
        "    Deviation %1%\n"
        "    Multiplier %2%\n"));
    m_info_text->set(0, 1.0f);
    m_info_text->set(1, 1.0f);

    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "Bloom parameters:", nullptr, vector4f(1,1,1,1)));
    add_text_source(m_info_text);
    add_text_sources(m_bright_pass->text_sources());
}

void bloom_parameters::on_set_current_time(float time)
{
    m_bright_pass->set_current_time(time);

    m_info_text->set(0, m_deviation->value_at(time));
    m_info_text->set(1, m_multiplier->value_at(time));
}

void bloom_parameters::on_handle_key_down(ui::key_down_event& event)
{
    // Bloom deviation BD+ BD-
    /*if (event.key_code() == VK_ADD && ui::is_key_down('B') && ui::is_key_down('D'))
        add_5_percent(*m_deviation, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('B') && ui::is_key_down('D'))
        dec_5_percent(*m_deviation, time());
    */
    // Bloom multiplier BM+ BM-
    /*if (event.key_code() == VK_ADD && ui::is_key_down('B') && ui::is_key_down('M'))
        add_5_percent(*m_multiplier, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('B') && ui::is_key_down('M'))
        dec_5_percent(*m_multiplier, time());
    */
    m_info_text->set(0, m_deviation->value_at(time()));
    m_info_text->set(1, m_multiplier->value_at(time()));

    m_bright_pass->handle_key_down(event);
}

void bloom_parameters::on_clear_overrides()
{
    m_deviation->set_value_source(nullptr);
    m_multiplier->set_value_source(nullptr);

    m_bright_pass->clear_overrides();
}

}} // namespace content::sequences
