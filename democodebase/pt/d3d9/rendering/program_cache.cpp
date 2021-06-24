#include "program_cache.h"
#include "shading_program_builder.h"
#include "renderer.h"
#include "pt/any_error.h"

namespace pt { namespace d3d9 { namespace rendering {

// program_cache::key

program_cache::key::key(
    shaders::surface_shader const& surface_shader,
    shaders::light_shader const& light_shader)
: surface_shader(&surface_shader)
, light_shader(&light_shader)
{
}

bool program_cache::key::operator <(key const& x) const
{
    // true if lhs < rhs, false otherwise.
    // If lhs and rhs are equial then comp(lhs, rhs) and comp(rhs, lhs) must both return false.

    if (surface_shader < x.surface_shader)
        return true;
    else if (surface_shader > x.surface_shader)
        return false;

    return light_shader < x.light_shader;
}

// program_cache

program_cache::program_cache(graphics_device& device)
: m_device(&device)
{
}

shared_shading_program program_cache::find_or_create(
    shaders::surface_shader const& surface_shader,
    shaders::light_shader const& light_shader)
{
    auto key = program_cache::key(surface_shader, light_shader);
    auto i = m_programs.find(key);
    if (i != m_programs.end())
        return i->second;

    auto new_program = create_shared_shading_program(
        *m_device, surface_shader, light_shader);

    m_programs[key] = new_program;
    return new_program;
}

shared_shading_program program_cache::program_for(
    shaders::surface_shader const& surface_shader,
    shaders::light_shader const& light_shader) const
{
    auto i = m_programs.find(key(surface_shader, light_shader));
    if (i != m_programs.end())
        return i->second;
	
    throw any_error("shading program not found for given surface shader");
}

shared_shading_program program_cache::find_or_create_depth(
    shaders::surface_shader const& surface_shader)
{
    auto key = program_cache::key(surface_shader, global_dummy_light_shader());
    auto i = m_depth_programs.find(key);
    if (i != m_depth_programs.end())
        return i->second;

    auto new_program = create_shared_depth_shading_program(
        *m_device, surface_shader);

    m_depth_programs[key] = new_program;
    return new_program;
}

shared_shading_program program_cache::depth_program_for(
    shaders::surface_shader const& surface_shader) const
{
    auto key = program_cache::key(surface_shader, global_dummy_light_shader());
    auto i = m_depth_programs.find(key);
    if (i != m_depth_programs.end())
        return i->second;

    throw any_error(
        "depth shading program not found for given surface shader");
}

}}} // namespace pt::d3d9::rendering
