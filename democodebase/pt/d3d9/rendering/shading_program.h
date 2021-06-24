#pragma once
#ifndef PT_D3D9_RENDERING_SHADING_PROGRAM_H
#define PT_D3D9_RENDERING_SHADING_PROGRAM_H

#include "shading_program_fwd.h"
#include "variable_owner_fwd.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/d3d9/pixel_shader_fwd.h"
#include "pt/d3d9/vertex_shader_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"
#include "pt/std/set.h"

namespace pt { namespace d3d9 { namespace rendering {

class shading_program
{
public:
    shading_program(graphics_device& device,
        std::shared_ptr<pixel_shader> pixel_shader,
        std::shared_ptr<vertex_shader> vertex_shader);
    
    void use();

    void create_resources(
        std::vector<variable_owner*> const& owners);
    void prepare(std::vector<variable_owner const*> const& owners, float time) const;

private:
    graphics_device* m_device;
	std::shared_ptr<pixel_shader> m_pixel_shader;
	std::shared_ptr<vertex_shader> m_vertex_shader;
    std::set<variable_owner*> m_prepared_owners;
};

using shared_shading_program = std::shared_ptr<shading_program>;

}}} // namespace pt::d3d9::rendering

#endif
