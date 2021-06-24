#include "dof_parameters.h"
#include "../utility.h"
#include "pt/ui/common.h"
#include "pt/ui/input_event.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/overridable_value_source.h"

namespace content { namespace sequences {

// bright_pass_parameters

dof_parameters::dof_parameters(config_node const& config,
    std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("depth_of_field")
{
    if (config.key() != "depth_of_field")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as depth_of_field");
    }

    // Create sources for parameter_node values.

    m_focus = create_overridable_value_source<float>(
        *config.child("focus"), curves);
    m_range = create_overridable_value_source<float>(
        *config.child("range"), curves);
	m_blur_rgb_dist = create_overridable_value_source<float>(
		*config.child("blur_rgb_dist"), curves);
	m_gain = create_overridable_value_source<float>(
		*config.child("gain"), curves);
	m_threshold = create_overridable_value_source<float>(
		*config.child("threshold"), curves);

    add(create_basic_node("focus", m_focus));
    add(create_basic_node("range", m_range));
	add(create_basic_node("blur_rgb_dist", m_blur_rgb_dist));
	add(create_basic_node("gain", m_gain));
	add(create_basic_node("threshold", m_threshold));
    // Create info text.

    m_info_text.reset(new d3d9::formatted_text_source<float,5>());
    m_info_text->set(boost::format(
        "    Focus: %1%\n"
        "    Range: %2%\n"
		"    Blur rgb distort: %3%\n"
		"    Gain: %4%\n"
		"    Threshold: %5%\n"
		));
    m_info_text->set(0, 1.0f);
    m_info_text->set(1, 1.0f);
	m_info_text->set(2, 1.0f);
	m_info_text->set(3, 1.0f);
	m_info_text->set(4, 1.0f);

    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "DoF parameters:", nullptr, vector4f(1,1,1,1)));
    add_text_source(m_info_text);
}

void dof_parameters::on_set_current_time(float time)
{
    m_info_text->set(0, m_focus->value_at(time));
    m_info_text->set(1, m_range->value_at(time));
	m_info_text->set(2, m_blur_rgb_dist->value_at(time));
	m_info_text->set(3, m_gain->value_at(time));
	m_info_text->set(4, m_threshold->value_at(time));
}

void dof_parameters::on_handle_key_down(ui::key_down_event& event)
{
    // Depth Of Field Focus DS+ DS- 
    if (ui::is_key_down('D') && ui::is_key_down('S'))
    {
        if (event.key_code() == VK_ADD)
        {
            add_5_percent(*m_focus, time());
        }
        else if (event.key_code() == VK_SUBTRACT)
        {
            dec_5_percent(*m_focus, time());
        }
    }

    // Depth of Field Range DA+ DA- 
    if (ui::is_key_down('D') && ui::is_key_down('A'))
    {
        if (event.key_code() == VK_ADD)
        {
            add_5_percent(*m_range, time());
        }
        else if (event.key_code() == VK_SUBTRACT)
        {
            dec_5_percent(*m_range, time());
        }
    }

	// Depth of Field blur rgb distort DQ+ DQ- 
	if (ui::is_key_down('D') && ui::is_key_down('Q'))
	{
		if (event.key_code() == VK_ADD)
		{
			add_5_percent(*m_blur_rgb_dist, time());
		}
		else if (event.key_code() == VK_SUBTRACT)
		{
			dec_5_percent(*m_blur_rgb_dist, time());
		}
	}

	// Depth of Field luminance gain DG+ DG- 
	if (ui::is_key_down('D') && ui::is_key_down('G'))
	{
		if (event.key_code() == VK_ADD)
		{
			//add_5_percent(*m_gain, time());
			float current = m_gain->value_at(time());
			current += 0.1f;
			m_gain->set(current);
		}
		else if (event.key_code() == VK_SUBTRACT)
		{
			dec_5_percent(*m_gain, time());
		}
	}
	// Depth of Field luminance threshold DG+ DG- 
	if (ui::is_key_down('D') && ui::is_key_down('T'))
	{
		if (event.key_code() == VK_ADD)
		{
			add_5_percent(*m_threshold, time());
		}
		else if (event.key_code() == VK_SUBTRACT)
		{
			dec_5_percent(*m_threshold, time());
		}
	}
    m_info_text->set(0, m_focus->value_at(time()));
    m_info_text->set(1, m_range->value_at(time()));
	m_info_text->set(2, m_blur_rgb_dist->value_at(time()));
	m_info_text->set(3, m_gain->value_at(time()));
	m_info_text->set(4, m_threshold->value_at(time()));
}

void dof_parameters::on_clear_overrides()
{
    m_focus->set_value_source(nullptr);
    m_range->set_value_source(nullptr);
	m_blur_rgb_dist->set_value_source(nullptr);
	m_gain->set_value_source(nullptr);
	m_threshold->set_value_source(nullptr);
}

}} // namespace content::sequences
