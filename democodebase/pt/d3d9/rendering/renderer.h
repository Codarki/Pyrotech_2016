#ifndef PT_D3D9_RENDERING_RENDERER_H
#define PT_D3D9_RENDERING_RENDERER_H

#include "pt/d3d9/shaders/light_shader_fwd.h"
#include "pt/d3d9/graphics_device_fwd.h"
//#include "pt/std/memory.h"
#include "pt/std/vector.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

struct draw_call;
class drawable;
class item;
class light;
class program_cache;
class shading_program;
class surface;
class variable_owner;
class view;

void render_depth(std::vector<draw_call> const& calls, float time,
    view* view, program_cache const& program_cache,
    d3d9::graphics_device& device);

void render(std::vector<draw_call> const& calls, float time, light* light,
    view* view, program_cache const& program_cache,
    d3d9::graphics_device& device,
    bool usePreshader = false);

void render_to_shadowmap(std::vector<draw_call> const& calls, float time,
	view* view, std::shared_ptr<shading_program> program);

/// Draw additive and alpha blended materials in one pass, sorted by depth.
void render_transparents(
    std::vector<draw_call> const& calls,
    float time,
    view* view,
    program_cache const& program_cache,
    item** items_with_light,
    size_t light_count,
    graphics_device& device);

void update(surface& surface,
    item* item_with_light,
    program_cache& program_cache,
	std::vector<variable_owner*> const& owners);

void update_shadow_depth(surface& surface,
	std::shared_ptr<d3d9::rendering::shading_program> shadow_depth_program,
	std::vector<variable_owner*> const& shadow_depth_owners);

// TODO: Move this function somewhere else.
shaders::light_shader const& global_dummy_light_shader();

}}} // namespace pt::d3d9::rendering

#endif
