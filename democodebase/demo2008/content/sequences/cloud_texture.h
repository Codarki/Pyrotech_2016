#pragma once
#ifndef CONTENT_SEQUENCES_CLOUD_TEXTURE_H
#define CONTENT_SEQUENCES_CLOUD_TEXTURE_H

#include "composite_sequence.h"
#include "pt/config_node_fwd.h"
#include "pt/parameter_collection_fwd.h"

namespace content { namespace sequences {

class cloud_texture : public composite_sequence
{
public:
    cloud_texture(
        //d3d9::graphics_device& device,
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
    class impl;
    std::unique_ptr<impl> m_impl;
};

}} // namespace content::sequences

#endif
