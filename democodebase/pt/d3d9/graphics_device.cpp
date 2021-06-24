#include "graphics_device.h"
#include "detail/state_manager.h"
#include "detail/vertex_declaration_cache.h"
#include "graphics_system.h"
#include "device_child.h"
#include "device_convenience.h"
#include "error.h"
#include "resource.h"
#include "texture_2d_collection.h"
#include "pt/any_error.h"
#include "pt/log.h"
#include "pt/format.h"

namespace pt { namespace d3d9 {

graphics_device::graphics_device(graphics_system& system,
    D3DPRESENT_PARAMETERS parameters, UINT adapter_index, HWND handle)
:   m_system(system)
,   m_parameters(parameters)
{
    log("Creating graphics device");

    com_ptr<IDirect3DDevice9> device;
    DWORD behavior_flags = 0;
    //behavior_flags |= D3DCREATE_NOWINDOWCHANGES;

    assert(m_system.native());

    try
    {
        check_dx(m_system.native()->CreateDevice(adapter_index,
            D3DDEVTYPE_HAL, handle,
            behavior_flags | D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &parameters, m_device.receive()),
            "IDirect3D9::CreateDevice");
    }
    catch(std::exception const&)
    {
        check_dx(m_system.native()->CreateDevice(adapter_index,
            D3DDEVTYPE_HAL, handle,
            behavior_flags | D3DCREATE_MIXED_VERTEXPROCESSING,
            &parameters, m_device.receive()),
            "IDirect3D9::CreateDevice");
    }

    m_state_manager.reset(new detail::state_manager(this));
    m_vertex_declarations.reset(new detail::vertex_declaration_cache(*this));
}

graphics_device::~graphics_device()
{
    set_stream_source(*this, 0, com_ptr<IDirect3DVertexBuffer9>(), 0, 0);
    //dynamic_vertex_buffer::clear_native_buffers();

    for (std::vector<device_child*>::iterator i = m_children.begin();
        i != m_children.end(); ++i)
    {
        (*i)->before_destruction();
    }

    m_device.reset();

    for (std::vector<device_child*>::iterator i = m_children.begin();
        i != m_children.end(); ++i)
    {
        (*i)->device_destructed();
    }

    d3d9::clear_default_texture_2d_collection();
    d3d9::clear_default_depth_stencil();

    log("Graphics device destroyed");
}

com_ptr<IDirect3DDevice9>& graphics_device::native()
{
    return m_device;
}

com_ptr<IDirect3DDevice9> const& graphics_device::native() const
{
    return m_device;
}

com_ptr<IDirect3DDevice9>& graphics_device::mutable_native() const
{
    return m_device;
}

void graphics_device::reset(int width, int height)
{
    log(format("Reseting graphics device to size [%1%, %2%]") % width % height);

    for (std::vector<device_child*>::iterator i = m_children.begin();
        i != m_children.end(); ++i)
    {
        (*i)->before_reset();
    }

    //m_before_destruction();

    m_parameters.BackBufferWidth = width;
    m_parameters.BackBufferHeight = height;

    check_dx(m_device->Reset(&m_parameters), "IDirect3D9::Reset");

    //m_after_reset();

    for (std::vector<device_child*>::iterator i = m_children.begin();
        i != m_children.end(); ++i)
    {
        (*i)->after_reset();
    }
}

D3DPRESENT_PARAMETERS const& graphics_device::parameters() const
{
    return m_parameters;
}

//graphics_device::before_destruction_signal&
//    graphics_device::before_destruction()
//{
//    return m_before_destruction;
//}
//
//graphics_device::after_reset_signal& graphics_device::after_reset()
//{
//    return m_after_reset;
//}

void graphics_device::add_child(device_child* child)
{
    m_children.push_back(child);
}

void graphics_device::remove_child(device_child* child)
{
    std::vector<device_child*>::iterator i
        = std::find(m_children.begin(), m_children.end(), child);
    if (i == m_children.end())
        throw any_error("device_child not found");
    m_children.erase(i);
}

D3DDEVTYPE graphics_device::type() const
{
    return D3DDEVTYPE_HAL;
}

detail::state_manager& graphics_device::state_manager()
{
    return *m_state_manager;
}

detail::state_manager const& graphics_device::state_manager() const
{
    return *m_state_manager;
}

detail::vertex_declaration_cache const&
    graphics_device::vertex_declarations() const
{
    return *m_vertex_declarations;
}

}} // namespace pt::d3d9
