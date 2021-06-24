#ifndef PT_D3D9_RENDERING_PROGRAM_CACHE_H
#define PT_D3D9_RENDERING_PROGRAM_CACHE_H

#include "shading_program_fwd.h"
#include "pt/d3d9/shaders/light_shader_fwd.h"
#include "pt/d3d9/shaders/surface_shader_fwd.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/std/map.h"

namespace pt { namespace d3d9 { namespace rendering {

/// Cache for shading programs. Does not take ownership of given shaders.
class program_cache final
{
public:
    explicit program_cache(graphics_device& device);

    shared_shading_program find_or_create(
        shaders::surface_shader const& surface_shader,
        shaders::light_shader const& light_shader);
    shared_shading_program program_for(
        shaders::surface_shader const& surface_shader,
        shaders::light_shader const& light_shader) const;

    // Depth programs shouldnt need light shader, right?
    shared_shading_program find_or_create_depth(
        shaders::surface_shader const& surface_shader);
    shared_shading_program depth_program_for(
        shaders::surface_shader const& surface_shader) const;

private: // type definitions
    struct key
    {
        key(shaders::surface_shader const& surface_shader,
            shaders::light_shader const& light_shader);

        shaders::surface_shader const* surface_shader;
        shaders::light_shader const* light_shader;
        bool operator<(key const& x) const;
    };
    typedef std::map<key,shared_shading_program> map;

private: // data members
    graphics_device* m_device;
    map m_programs;
    map m_depth_programs;
};

}}} // namespace pt::d3d9::rendering

#endif
