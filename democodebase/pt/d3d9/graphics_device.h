#ifndef PT_D3D9_GRAPHICS_DEVICE_H
#define PT_D3D9_GRAPHICS_DEVICE_H

#include "pt/boost/noncopyable.h"
#include "native.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {

class device_child;
class graphics_system;
class resource;

namespace detail {
    class state_manager;
    class vertex_declaration_cache;
}

/// Graphics device for an adapter. Every device can have their own swap chain
/// (front and back buffers).
class graphics_device : boost::noncopyable
{
public:
    graphics_device(graphics_system& system,
        D3DPRESENT_PARAMETERS parameters, UINT adapter_index, HWND handle);
    ~graphics_device();

    /// \return Native Direct3D device interface.
    com_ptr<IDirect3DDevice9>& native();
    com_ptr<IDirect3DDevice9> const& native() const;
    com_ptr<IDirect3DDevice9>& mutable_native() const;

    void reset(int width, int height);

    D3DPRESENT_PARAMETERS const& parameters() const;

    void add_child(device_child* child);
    void remove_child(device_child* child);

    D3DDEVTYPE type() const;

    detail::state_manager& state_manager();
    detail::state_manager const& state_manager() const;

    detail::vertex_declaration_cache const& vertex_declarations() const;

private: // data members
    graphics_system& m_system;
    D3DPRESENT_PARAMETERS m_parameters;
    mutable com_ptr<IDirect3DDevice9> m_device;

    std::vector<device_child*> m_children;
	std::unique_ptr<detail::state_manager> m_state_manager;
    std::unique_ptr<detail::vertex_declaration_cache> m_vertex_declarations;
};

}} // namespace pt::d3d9

#endif
