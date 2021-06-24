#pragma once
#ifndef PT_D3D9_TEXTURE_2D_DATA_SET_BUILDER_H
#define PT_D3D9_TEXTURE_2D_DATA_SET_BUILDER_H

#include "pt/math/step_curve.h"
#include "graphics_device_fwd.h"

namespace pt { namespace d3d9 {

class texture_2d_data_set;

//class texture_2d_data_set_builder
//{
//public:
//    texture_2d_data_set_builder(
//        pt::math::step_curve_string const& texture_name_curve);
//
//    texture_2d_data_set build() const;
//};

texture_2d_data_set build_texture_2d_data_set(
    pt::math::step_curve_string_value_source const& value_source,
    graphics_device& device);

texture_2d_data_set build_texture_2d_data_set(
    pt::math::step_curve_string const& texture_name_step_curve,
    graphics_device& device);

}} // namespace pt::d3d9

#endif
