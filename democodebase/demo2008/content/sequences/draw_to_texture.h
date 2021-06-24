#pragma once
#ifndef CONTENT_SEQUENCES_DRAW_TO_TEXTURE_H
#define CONTENT_SEQUENCES_DRAW_TO_TEXTURE_H

#include "basic_composite_sequence.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/d3d9/texture_2d_fwd.h"
#include "pt/animatable_value.h"
#include "pt/vector.h"
#include "pt/parameter_collection_fwd.h"

namespace content { namespace sequences {

class draw_to_texture : public composite_sequence
{
public:
    draw_to_texture(
        pt::d3d9::graphics_device& device,
        pt::config_node const& config,
        pt::parameter_collection const& shared_parameters);

    draw_to_texture(
        pt::config_node const& config,
        std::shared_ptr<pt::d3d9::texture_2d> texture);

public: // interface required by composite_sequence
    pt::string_wrapper const& name() const;
    std::shared_ptr<pt::d3d9::texture_2d> draw(rendertarget& target, float time);
    float start_time() const;
    float end_time() const;
    void set_override_view(pt::d3d9::rendering::item* view);
    void link_inputs(composite_sequence_collection const& sequences);

private: // data members
    basic_composite_sequence m_basic;
    pt::vector2i m_resolution;
    int m_bits_per_channel;
    std::shared_ptr<pt::d3d9::texture_2d> m_texture;
    std::shared_ptr<pt::animatable_value<pt::vector4f> const > m_clear_color;
    bool m_is_clear_color_enabled;
};

std::shared_ptr<pt::d3d9::texture_2d> find_shared_texture(
    pt::config_node const& config,
    pt::parameter_collection const& shared_parameters,
    std::string const& xml_element_name);

}} // namespace content::sequences

#endif
