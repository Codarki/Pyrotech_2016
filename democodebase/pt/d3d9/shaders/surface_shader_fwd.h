#pragma once
#ifndef PT_D3D9_SHADERS_SURFACE_SHADER_FWD_H
#define PT_D3D9_SHADERS_SURFACE_SHADER_FWD_H

#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {

//class stream_collection;
class surface_shader;
using shared_surface_shader = std::shared_ptr<surface_shader>;
//class surface_shader_collection;

}}} // namespace pt::d3d9::shaders

#endif
