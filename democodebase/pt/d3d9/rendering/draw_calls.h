#ifndef PT_D3D9_RENDERING_DRAW_CALLS_H
#define PT_D3D9_RENDERING_DRAW_CALLS_H

#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class drawable;
class geometry;
class item;
class item_geometry;
class light;
class surface_variables_instance;
class transformation_variables;
class transformer;
class view;

struct draw_call
{
    draw_call(
        rendering::drawable const* drawable,
        rendering::surface_variables_instance const* surface_variables_instance,
        transformation_variables const* variables,
        d3d9::rendering::transformer const* transformer);

    drawable const* drawable;
    surface_variables_instance const* surface_variables_instance;
    transformation_variables const* variables;
    d3d9::rendering::transformer const* transformer;
};


void push_back_draw_calls(d3d9::rendering::item& item,
    std::vector<draw_call>& draw_calls, light* light);

//std::vector<draw_call> gather_draw_calls(item& root);
std::vector<draw_call> gather_opaque_draw_calls(item& root, float time);

std::vector<draw_call> gather_transparent_draw_calls(item& root, float time);
std::vector<draw_call> gather_additive_draw_calls(item& root, float time);

void sort_draw_calls(std::vector<draw_call>& calls);

void sort_transparent_draw_calls(item const& camera_item, float time,
    std::vector<draw_call>& calls);

}}} // namespace pt::d3d9::rendering

#endif
