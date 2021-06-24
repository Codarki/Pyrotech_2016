#include "light_scattering_parameters.h"
#include "../utility.h"
#include "pt/ui/common.h"
#include "pt/ui/input_event.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/overridable_value_source.h"

namespace content { namespace sequences {

// light_scattering_parameters

light_scattering_parameters::light_scattering_parameters(config_node const& config,
    std::shared_ptr<parameter_node> curves)
:   post_processing_parameters_base("light_scattering")
{
    if (config.key() != "light_scattering")
    {
        throw any_error("invalid config \"" + config.key()
            + "\"given as light_scattering");
    }

    // Create sources for parameter_node values.
	m_enabled = create_overridable_value_source<float>(
		*config.child("enabled"), curves);

	m_exposure = create_overridable_value_source<float>(
        *config.child("exposure"), curves);
    m_decay = create_overridable_value_source<float>(
        *config.child("decay"), curves);
	m_density = create_overridable_value_source<float>(
		*config.child("density"), curves);
	m_weight = create_overridable_value_source<float>(
		*config.child("weight"), curves);
	m_lightScrPosX = create_overridable_value_source<float>(
		*config.child("lightScrPosX"), curves);
	m_lightScrPosY = create_overridable_value_source<float>(
		*config.child("lightScrPosY"), curves);


	add(create_basic_node("enabled", m_enabled));
    add(create_basic_node("exposure", m_exposure));
    add(create_basic_node("decay", m_decay));
	add(create_basic_node("density", m_density));
	add(create_basic_node("weight", m_weight));
	add(create_basic_node("lightScrPosX", m_lightScrPosX));
	add(create_basic_node("lightScrPosY", m_lightScrPosY));
    // Create info text.

    m_info_text.reset(new d3d9::formatted_text_source<float,7>());
    m_info_text->set(boost::format(
		"    Enabled: %1%\n"
        "    Exposure: %2%\n"
        "    Decay: %3%\n"
		"    Density: %4%\n"
		"    Weight: %5%\n"
		"    Light Pos.(x,y): (%6%,%7%)\n"
		));
    m_info_text->set(0, 1.0f);
    m_info_text->set(1, 1.0f);
	m_info_text->set(2, 1.0f);
	m_info_text->set(3, 1.0f);
	m_info_text->set(4, 1.0f);
	m_info_text->set(5, 1.0f);
	m_info_text->set(6, 1.0f);

    add_text_source(std::make_shared<d3d9::basic_text_source>(
        "Light Scattering parameters:", nullptr, vector4f(1,1,1,1)));
    add_text_source(m_info_text);
}

void light_scattering_parameters::on_set_current_time(float time)
{
	m_info_text->set(0, m_enabled->value_at(time));
    m_info_text->set(1, m_exposure->value_at(time));
    m_info_text->set(2, m_decay->value_at(time));
	m_info_text->set(3, m_density->value_at(time));
	m_info_text->set(4, m_weight->value_at(time));
	m_info_text->set(5, m_lightScrPosX->value_at(time));
	m_info_text->set(6, m_lightScrPosY->value_at(time));
}

void light_scattering_parameters::on_handle_key_down(ui::key_down_event& event)
{
	// Enabled SHIFT-1
	if (ui::is_key_down(VK_SHIFT) && ui::is_key_down('1'))
	{
		int current = static_cast<int>(m_enabled->value_at(time()));
		current = !current;
		m_enabled->set(static_cast<float>(current));
	}

    // Exposure SHIFT-E+ SHIFT-E- 
	if (ui::is_key_down(VK_SHIFT) && ui::is_key_down('E'))
    {
        if (event.key_code() == VK_ADD)
        {
            add_5_percent(*m_exposure, time());
        }
        else if (event.key_code() == VK_SUBTRACT)
        {
            dec_5_percent(*m_exposure, time());
        }
    }

    // Decay SHIFT-D+ SHIFT-D- 
	if (ui::is_key_down(VK_SHIFT) && ui::is_key_down('D'))
    {
        if (event.key_code() == VK_ADD)
        {
			float current = m_decay->value_at(time());
			current += 0.001f;
			m_decay->set(current);
        }
        else if (event.key_code() == VK_SUBTRACT)
        {
			float current = m_decay->value_at(time());
			current -= 0.001f;
			m_decay->set(current);
        }
    }

	// Density SHIFT-X+ SHIFT-X- 
	if (ui::is_key_down(VK_SHIFT) && ui::is_key_down('X'))
	{
		if (event.key_code() == VK_ADD)
		{
			add_5_percent(*m_density, time());
		}
		else if (event.key_code() == VK_SUBTRACT)
		{
			dec_5_percent(*m_density, time());
		}
	}
	
	// Weight SHIFT-Q+ SHIFT-Q- 
	if (ui::is_key_down(VK_SHIFT) && ui::is_key_down('Q'))
	{
		if (event.key_code() == VK_ADD)
		{
			//add_5_percent(*m_gain, time());
			float current = m_weight->value_at(time());
			current += 0.1f;
			m_weight->set(current);
		}
		else if (event.key_code() == VK_SUBTRACT)
		{
			dec_5_percent(*m_weight, time());
		}
	}
	// Light X Pos
	{
		if (event.key_code() == VK_NUMPAD4)
		{
			float current = m_lightScrPosX->value_at(time());
			current += 0.01f;
			m_lightScrPosX->set(current);
		}
		else if (event.key_code() == VK_NUMPAD6)
		{
			float current = m_lightScrPosX->value_at(time());
			current -= 0.01f;
			m_lightScrPosX->set(current);
		}
	}

	// Light Y Pos
	{
		if (event.key_code() == VK_NUMPAD5)
		{
			float current = m_lightScrPosY->value_at(time());
			current += 0.01f;
			m_lightScrPosY->set(current);
		}
		else if (event.key_code() == VK_NUMPAD8)
		{
			float current = m_lightScrPosY->value_at(time());
			current -= 0.01f;
			m_lightScrPosY->set(current);
		}
	}

	m_info_text->set(0, m_enabled->value_at(time()));
	m_info_text->set(1, m_exposure->value_at(time()));
	m_info_text->set(2, m_decay->value_at(time()));
	m_info_text->set(3, m_density->value_at(time()));
	m_info_text->set(4, m_weight->value_at(time()));
	m_info_text->set(5, m_lightScrPosX->value_at(time()));
	m_info_text->set(6, m_lightScrPosY->value_at(time()));
}

void light_scattering_parameters::on_clear_overrides()
{
	m_enabled->set_value_source(nullptr);
    m_exposure->set_value_source(nullptr);
    m_decay->set_value_source(nullptr);
	m_density->set_value_source(nullptr);
	m_weight->set_value_source(nullptr);
	m_lightScrPosX->set_value_source(nullptr);
	m_lightScrPosY->set_value_source(nullptr);
}

}} // namespace content::sequences
