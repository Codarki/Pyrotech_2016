#ifndef PT_D3D9_RENDERING_DRAWABLE_BUILDER_H
#define PT_D3D9_RENDERING_DRAWABLE_BUILDER_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {
    class vertex_streams;
}}

namespace pt { namespace d3d9 { namespace rendering {

class basic_drawable;

std::shared_ptr<basic_drawable> build_shared_basic_drawable(
    graphics_device& device,
	std::shared_ptr<vertex_streams> const& streams,
    std::vector<size_t> const& indices);

}}} // namespace pt::d3d9::rendering

#endif
