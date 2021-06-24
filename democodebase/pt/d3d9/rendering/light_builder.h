#pragma once
#ifndef PT_D3D9_RENDERING_LIGHT_BUILDER_H
#define PT_D3D9_RENDERING_LIGHT_BUILDER_H

//#include "item.h"
//#include "pt/box.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/parameter_collection_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class light;
class material_cache;
class transformation;

std::unique_ptr<light> create_unique_light(
    transformation const& transformation,
    parameter_collection const& parameters,
    material_cache& material_cache,
    graphics_device& device);

//std::unique_ptr<light> create_unique_light(
//    transformation const& transformation,
//    parameter_collection const& parameters);

}}} // namespace pt::d3d9::rendering

#endif
