#pragma once
#ifndef CONTENT_SEQUENCES_POST_PROCESSING_H
#define CONTENT_SEQUENCES_POST_PROCESSING_H

#include "simple_composite_sequence.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/animatable_value_fwd.h"
#include "pt/parameter_collection_fwd.h"
#include "pt/ui/input_event_fwd.h"

namespace pt { namespace d3d9 { namespace rendering {
    class post_processing;
}}}

namespace content { namespace sequences {

class post_processing : public composite_sequence
{
public:
    post_processing(
        pt::d3d9::graphics_device& device,
        pt::config_node const& config,
        pt::parameter_collection const& parameters);

public: // interface required by composite_sequence
    pt::string_wrapper const& name() const;
    std::shared_ptr<pt::d3d9::texture_2d> draw(rendertarget& target, float time);
    float start_time() const;
    float end_time() const;
    void set_override_view(pt::d3d9::rendering::item* view);
    void link_inputs(composite_sequence_collection const& sequences);

private: // data members
    basic_composite_sequence m_basic;
    std::shared_ptr<pt::d3d9::rendering::post_processing> m_post_processing;
    float m_time;
    std::shared_ptr<pt::animatable_value<pt::vector3f> const > m_clear_color;
};

}} // namespace content::sequences

#endif
