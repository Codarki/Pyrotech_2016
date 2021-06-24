#pragma once
#ifndef PT_D3D9_RENDERING_SHADING_PROGRAM_BUILDER_H
#define PT_D3D9_RENDERING_SHADING_PROGRAM_BUILDER_H

#include "shading_program_fwd.h"
#include "pt/d3d9/shaders/light_shader_fwd.h"
#include "pt/d3d9/shaders/surface_shader_fwd.h"
#include "pt/d3d9/graphics_device_fwd.h"

namespace pt { namespace d3d9 { namespace rendering {

shared_shading_program create_shared_shading_program(
    graphics_device& device,
    shaders::surface_shader const& surface_shader,
    shaders::light_shader const& light_shader);

shared_shading_program create_shared_depth_shading_program(
    graphics_device& device,
    shaders::surface_shader const& surface_shaderr);

}}} // namespace pt::d3d9::rendering

#endif
