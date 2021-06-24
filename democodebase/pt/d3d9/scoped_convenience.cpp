#include "scoped_convenience.h"
#include "device_convenience.h"
#include "error.h"
#include "graphics_device.h"
#include "pt/log.h"

namespace pt { namespace d3d9 {

// scoped_scene

scoped_scene::scoped_scene(graphics_device& device)
:   device_reference(device)
{
    begin_scene(device);
}

scoped_scene::~scoped_scene()
{
    try
    {
	    end_scene(device());
    }
    catch(...)
    {
        log("exception in ~scoped_scene");
    }
}

// scoped_render_target

scoped_render_target::scoped_render_target(graphics_device& device,
    com_ptr<IDirect3DSurface9> surf, DWORD index)
:   device_reference(device)
,	m_index(index)
,	m_old(render_target(device, index))
{
	set_render_target(device, surf, index);
}

scoped_render_target::~scoped_render_target()
{
    try
    {
	    set_render_target(device(), m_old, m_index);
    }
    catch(...)
    {
        log("exception in ~scoped_render_target");
    }
}

// scoped_depth_stencil

scoped_depth_stencil::scoped_depth_stencil(graphics_device& device,
    com_ptr<IDirect3DSurface9> surface)
:   device_reference(device)
{
    m_old = optional_depth_stencil_surface(device);
	set_depth_stencil_surface(device, surface);
}

scoped_depth_stencil::~scoped_depth_stencil()
{
    try
    {
	    set_depth_stencil_surface(device(), m_old);
    }
    catch(...)
    {
        log("exception in ~scoped_depth_stencil");
    }
}

// scoped_render_state

scoped_render_state::scoped_render_state(graphics_device& device,
    D3DRENDERSTATETYPE state, DWORD value)
:   device_reference(device)
,   m_state(state)
,   m_old_value(render_state(device, state))
{
    set_render_state(device, m_state, value);
}

scoped_render_state::~scoped_render_state()
{
    try
    {
	    set_render_state(device(), m_state, m_old_value);
    }
    catch(...)
    {
        log("exception in ~scoped_render_state");
    }
}

// scoped_texture_2d_lock

scoped_texture_2d_lock::scoped_texture_2d_lock(
    com_ptr<IDirect3DTexture9> texture)
: m_texture(texture)
{
    UINT level = 0;
    const RECT* rect = nullptr;
    DWORD flags = D3DLOCK_DISCARD;

    assert(m_texture);
    check_dx(m_texture->LockRect(level, &locked_rect, rect, flags),
        "IDirect3DTexture9::LockRect");
}

scoped_texture_2d_lock::~scoped_texture_2d_lock()
{
    UINT level = 0;
    check_dx(m_texture->UnlockRect(level),
        "IDirect3DTexture9::UnlockRect");
}

void* scoped_texture_2d_lock::data()
{
    return locked_rect.pBits;
}

// scoped_texture_stage_state

scoped_texture_stage_state::scoped_texture_stage_state(
    graphics_device& device, DWORD stage, D3DTEXTURESTAGESTATETYPE state,
    DWORD value)
:   device_reference(device)
,   m_stage(stage)
,   m_state(state)
,   m_old_value(texture_stage_state(device, stage, state))
{
    set_texture_stage_state(device, m_stage, m_state, value);
}

scoped_texture_stage_state::~scoped_texture_stage_state()
{
    try
    {
	    set_texture_stage_state(device(), m_stage, m_state, m_old_value);
    }
    catch(...)
    {
        log("exception in ~scoped_texture_stage_state");
    }
}

// scoped_viewport

scoped_viewport::scoped_viewport(graphics_device& device,
    D3DVIEWPORT9 const& viewport)
:   device_reference(device)
,   m_old_viewport(d3d9::viewport(device))
{
    set_viewport(device, viewport);
}

scoped_viewport::~scoped_viewport()
{
    try
    {
	    set_viewport(device(), m_old_viewport);
    }
    catch(...)
    {
        log("exception in ~scoped_viewport");
    }
}

// vertex_buffer_lock

vertex_buffer_lock::vertex_buffer_lock(
    com_ptr<IDirect3DVertexBuffer9> buffer, UINT offset_to_lock,
    UINT size_to_lock, DWORD flags)
:   m_buffer(buffer)
{
    assert(m_buffer);
    check_dx(m_buffer->Lock(offset_to_lock, size_to_lock, &m_data, flags),
        "IDirect3DVertexBuffer9::Lock");
}

vertex_buffer_lock::~vertex_buffer_lock()
{
    m_buffer->Unlock();
}

void* vertex_buffer_lock::data() const
{
    return m_data;
}

}} // namespace d3d9
