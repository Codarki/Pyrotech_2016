#ifndef CONTENT_SEQUENCES_POST_PROCESSING_PARAMETERS_H
#define CONTENT_SEQUENCES_POST_PROCESSING_PARAMETERS_H

#include "post_processing_parameters_base.h"
#include "pt/d3d9/formatted_text_source.h"
#include "pt/animatable_value_source_fwd.h"
#include "pt/overridable_value_source_fwd.h"
#include "pt/ui/input_event_fwd.h"

namespace content { namespace sequences {

using namespace pt;

class post_processing_parameters : public post_processing_parameters_base
{
public:
    post_processing_parameters(config_node const& config,
        std::shared_ptr<parameter_node> curves);

protected: // post_processing_parameters_base virtual function overloads
    void on_set_current_time(float time);
    void on_handle_key_down(ui::key_down_event& event);
    void on_clear_overrides();

private: // data members
	std::shared_ptr<post_processing_parameters_base> m_bloom;
    std::shared_ptr<post_processing_parameters_base> m_tone_mapping;
    std::shared_ptr<post_processing_parameters_base> m_noise;
    std::shared_ptr<post_processing_parameters_base> m_dof;
	std::shared_ptr<post_processing_parameters_base> m_light_scattering;

    float m_time;
};

class bright_pass_parameters : public post_processing_parameters_base
{
public:
    bright_pass_parameters(config_node const& config,
        std::shared_ptr<parameter_node> curves);

protected: // post_processing_parameters_base virtual function overloads
    void on_set_current_time(float time);
    void on_handle_key_down(ui::key_down_event& event);
    void on_clear_overrides();

private: // data members
    std::shared_ptr<overridable_value_source<float> > m_adapted_luminance;
    std::shared_ptr<overridable_value_source<float> > m_middle_gray;
    std::shared_ptr<overridable_value_source<float> > m_threshold;
    std::shared_ptr<overridable_value_source<float> > m_offset;
    std::shared_ptr<overridable_value_source<float> > m_white_point;
    boost::shared_ptr<d3d9::formatted_text_source<float,5> > m_info_text;
};

class noise_parameters : public post_processing_parameters_base
{
public:
    noise_parameters(config_node const& config,
        std::shared_ptr<parameter_node> curves);

protected: // post_processing_parameters_base virtual function overloads
    void on_set_current_time(float time);
    void on_handle_key_down(ui::key_down_event& event);
    void on_clear_overrides();

private: // data members
    std::shared_ptr<overridable_value_source<float> > m_noise_strength;
    std::shared_ptr<overridable_value_source<float> > m_noise_speed;
    std::shared_ptr<overridable_value_source<float> > m_noise_frequency;
    
    std::shared_ptr<d3d9::formatted_text_source<float, 2> > m_info_text;
};

class tone_mapping_parameters : public post_processing_parameters_base
{
public:
    tone_mapping_parameters(config_node const& config,
        std::shared_ptr<parameter_node> curves);

protected: // post_processing_parameters_base virtual function overloads
    void on_set_current_time(float time);
    void on_handle_key_down(ui::key_down_event& event);
    void on_clear_overrides();

private: // data members
    std::shared_ptr<overridable_value_source<float> > m_adapted_luminance;
    std::shared_ptr<overridable_value_source<float> > m_middle_gray;
    std::shared_ptr<overridable_value_source<float> > m_saturation;
    std::shared_ptr<overridable_value_source<float> > m_white_point;
    std::shared_ptr<overridable_value_source<pt::vector3f>> m_color_correction;

    std::shared_ptr<overridable_value_source<pt::vector3f>> m_negation_color;
    std::shared_ptr<overridable_value_source<pt::vector4f>> m_attribute1;
    std::shared_ptr<overridable_value_source<pt::vector4f>> m_attribute2;
    std::shared_ptr<overridable_value_source<pt::vector4f>> m_attribute3;
    std::shared_ptr<overridable_value_source<pt::vector4f>> m_attribute4;
    std::shared_ptr<overridable_value_source<pt::vector4f>> m_attribute5;

    std::shared_ptr<d3d9::formatted_text_source<float, 4> > m_info_text;
};

}} // namespace content::sequences

#endif
