#include "bright_pass_parameters.h"
#include "../utility.h"
#include "pt/ui/common.h"
#include "pt/ui/input_event.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/overridable_value_source.h"

namespace content { namespace sequences {

// bright_pass_parameters

bright_pass_parameters::bright_pass_parameters(config_node const& config,
    std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("bright_pass")
{
    if (config.key() != "bright_pass")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as bright_pass");
    }

    // Create sources for parameter_node values.

    m_adapted_luminance = create_overridable_value_source<float>(
        *config.child("adapted_luminance"), curves);
    m_middle_gray = create_overridable_value_source<float>(
        *config.child("middle_gray"), curves);
    m_threshold = create_overridable_value_source<float>(
        *config.child("threshold"), curves);
    m_offset = create_overridable_value_source<float>(
        *config.child("offset"), curves);
    m_white_point = create_overridable_value_source<float>(
        *config.child("white_point"), curves);

    add(create_basic_node("adapted_luminance", m_adapted_luminance));
    add(create_basic_node("middle_gray", m_middle_gray));
    add(create_basic_node("threshold", m_threshold));
    add(create_basic_node("offset", m_offset));
    add(create_basic_node("white_point", m_white_point));

    // Create info text.

    m_info_text.reset(new d3d9::formatted_text_source<float,5>());
    m_info_text->set(boost::format(
        "    Adapted luminance: %1%\n"
        "    Middle gray: %2%\n"
        "    Bright pass threshold: %3%\n"
        "    Bright pass offset: %4%\n"
        "    Bright pass white point: %5%"));
    m_info_text->set(0, 1.0f);
    m_info_text->set(1, 1.0f);
    m_info_text->set(2, 1.0f);
    m_info_text->set(3, 1.0f);
    m_info_text->set(4, 1.0f);

    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "Bright pass parameters:", nullptr, vector4f(1, 1, 1, 1)));
    add_text_source(m_info_text);
}

void bright_pass_parameters::on_set_current_time(float time)
{
    m_info_text->set(0, m_adapted_luminance->value_at(time));
    m_info_text->set(1, m_middle_gray->value_at(time));
    m_info_text->set(2, m_threshold->value_at(time));
    m_info_text->set(3, m_offset->value_at(time));
    m_info_text->set(4, m_white_point->value_at(time));
}

void bright_pass_parameters::on_handle_key_down(ui::key_down_event& event)
{
    // Adapted luminance L+ L- 
    if (event.key_code() == VK_ADD && ui::is_key_down('L'))
        add_5_percent(*m_adapted_luminance, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('L'))
        dec_5_percent(*m_adapted_luminance, time());

    // Middle gray G+ G-
    //if (event.key_code() == VK_ADD && ui::is_key_down('G'))
    //    add_5_percent(*m_middle_gray, time());
    //else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('G'))
    //    dec_5_percent(*m_middle_gray, time());

    // Bright pass threshold BT+ BT-
    if (event.key_code() == VK_ADD && ui::is_key_down('B') && ui::is_key_down('T'))
        add_5_percent(*m_threshold, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('B') && ui::is_key_down('T'))
        dec_5_percent(*m_threshold, time());

    // Bright pass offset BO+ BO-
    if (event.key_code() == VK_ADD && ui::is_key_down('B') && ui::is_key_down('O'))
        add_5_percent(*m_offset, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('B') && ui::is_key_down('O'))
        dec_5_percent(*m_offset, time());

    // Bright pass white point BW+ BW-
    if (event.key_code() == VK_ADD && ui::is_key_down('B') && ui::is_key_down('W'))
        add_5_percent(*m_white_point, time());
    else if (event.key_code() == VK_SUBTRACT && ui::is_key_down('B') && ui::is_key_down('W'))
        dec_5_percent(*m_white_point, time());

    m_info_text->set(0, m_adapted_luminance->value_at(time()));
    m_info_text->set(1, m_middle_gray->value_at(time()));
    m_info_text->set(2, m_threshold->value_at(time()));
    m_info_text->set(3, m_offset->value_at(time()));
    m_info_text->set(4, m_white_point->value_at(time()));
}

void bright_pass_parameters::on_clear_overrides()
{
    m_adapted_luminance->set_value_source(nullptr);
    m_middle_gray->set_value_source(nullptr);
    m_threshold->set_value_source(nullptr);
    m_offset->set_value_source(nullptr);
    m_white_point->set_value_source(nullptr);
}

}} // namespace content::sequences
