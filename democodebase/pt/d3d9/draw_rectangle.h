#ifndef PT_D3D9_DRAW_RECTANGLE_H
#define PT_D3D9_DRAW_RECTANGLE_H

#include "graphics_device_fwd.h"
#include "pt/box.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 {

void draw_rectangle(graphics_device& device, box2f const& area,
    vector4f const& color);
void draw_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector4f const& color);

}} // namespace pt::d3d9

#endif
