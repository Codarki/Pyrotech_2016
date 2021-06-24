#ifndef CONTENT_SEQUENCES_LIGHT_SCATTERING_PARAMETERS_H
#define CONTENT_SEQUENCES_LIGHT_SCATTERING_PARAMETERS_H

#include "post_processing_parameters_base.h"
#include "pt/d3d9/formatted_text_source.h"
#include "pt/animatable_value_source_fwd.h"
#include "pt/overridable_value_source_fwd.h"
#include "pt/ui/input_event_fwd.h"

namespace content { namespace sequences {

using namespace pt;

class light_scattering_parameters : public post_processing_parameters_base
{
public:
	light_scattering_parameters(config_node const& config,
        std::shared_ptr<parameter_node> curves);

protected: // post_processing_parameters_base virtual function overloads
    void on_set_current_time(float time);
    void on_handle_key_down(ui::key_down_event& event);
    void on_clear_overrides();

private: // data members
	std::shared_ptr<overridable_value_source<float> > m_enabled;

    std::shared_ptr<overridable_value_source<float> > m_exposure;
    std::shared_ptr<overridable_value_source<float> > m_decay;
	std::shared_ptr<overridable_value_source<float> > m_density;
	std::shared_ptr<overridable_value_source<float> > m_weight;
	std::shared_ptr<overridable_value_source<float> > m_lightScrPosX;
	std::shared_ptr<overridable_value_source<float> > m_lightScrPosY;
	std::shared_ptr<d3d9::formatted_text_source<float, 7> > m_info_text;
};

}} // namespace content::sequences

#endif
