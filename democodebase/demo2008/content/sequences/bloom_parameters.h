#ifndef CONTENT_SEQUENCES_BLOOM_PARAMETERS_H
#define CONTENT_SEQUENCES_BLOOM_PARAMETERS_H

#include "post_processing_parameters_base.h"
#include "pt/d3d9/formatted_text_source.h"
#include "pt/animatable_value_source_fwd.h"
#include "pt/overridable_value_source_fwd.h"
#include "pt/ui/input_event_fwd.h"

namespace pt { namespace d3d9 {
    class texture_2d;
}}

namespace content { namespace sequences {

using namespace pt;

class bloom_parameters : public post_processing_parameters_base
{
public:
    bloom_parameters(config_node const& config,
        std::shared_ptr<parameter_node> curves);

protected: // post_processing_parameters_base virtual function overloads
    void on_set_current_time(float time);
    void on_handle_key_down(ui::key_down_event& event);
    void on_clear_overrides();

private: // data members
    std::shared_ptr<post_processing_parameters_base> m_bright_pass;
    std::shared_ptr<overridable_value_source<float> > m_deviation;
    std::shared_ptr<overridable_value_source<float> > m_multiplier;
    //std::shared_ptr<animatable_value_source<boost::shared_ptr<pt::d3d9::texture_2d>> > m_blur_strength;
    std::shared_ptr<animatable_value_source<std::string> > m_blur_strength;
	std::shared_ptr<animatable_value_source<std::string> > m_horizontal_strength;
	std::shared_ptr<d3d9::formatted_text_source<float, 2> > m_info_text;
};

}} // namespace content::sequences

#endif
