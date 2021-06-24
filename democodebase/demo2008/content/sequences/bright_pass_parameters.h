#ifndef CONTENT_SEQUENCES_BRIGHT_PASS_PARAMETERS_H
#define CONTENT_SEQUENCES_BRIGHT_PASS_PARAMETERS_H

#include "post_processing_parameters_base.h"
#include "pt/d3d9/formatted_text_source.h"
#include "pt/animatable_value_source_fwd.h"
#include "pt/overridable_value_source_fwd.h"
#include "pt/ui/input_event_fwd.h"

namespace content { namespace sequences {

using namespace pt;

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
	std::shared_ptr<d3d9::formatted_text_source<float, 5> > m_info_text;
};

}} // namespace content::sequences

#endif
