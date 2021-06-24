#pragma once
#ifndef CONTENT_SEQUENCES_DEPTH_BUFFER_H
#define CONTENT_SEQUENCES_DEPTH_BUFFER_H

#include "basic_composite_sequence.h"
#include "pt/com_ptr.h"
#include "pt/d3d9/native.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/animatable_value_fwd.h"
#include "pt/vector.h"
#include "pt/parameter_collection_fwd.h"

namespace content { namespace sequences {

using namespace pt;

class depth_buffer : public composite_sequence
{
public:
    depth_buffer(
        d3d9::graphics_device& device,
        config_node const& config,
        parameter_collection const& parameters);

public: // interface required by composite_sequence
    string_wrapper const& name() const;
	std::shared_ptr<d3d9::texture_2d> draw(rendertarget& target, float time);
    float start_time() const;
    float end_time() const;
    void set_override_view(d3d9::rendering::item* view);
    void link_inputs(composite_sequence_collection const& sequences);

private: // data members
    basic_composite_sequence m_basic;
    d3d9::graphics_device* m_device;
    com_ptr<IDirect3DSurface9> m_depth_stencil;
    std::shared_ptr<animatable_value<vector3f> const > m_clear_color;
};

}} // namespace content::sequences

#endif
