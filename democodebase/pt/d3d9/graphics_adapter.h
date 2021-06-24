#ifndef PT_D3D9_GRAPHICS_ADAPTER_H
#define PT_D3D9_GRAPHICS_ADAPTER_H

#include "pt/boost/noncopyable.h"
#include "native.h"
#include "graphics_device_fwd.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {

class graphics_system;

enum swapeffect
{
    swap_copy,
    swap_discard,
    swap_flip
};

struct graphics_device_parameters
{
    graphics_device_parameters();

    int width;
    int height;
    int bits;
    int refresh_rate;
    int aa_count;
    int aa_quality;
    bool fullscreen;
    swapeffect swap;
};

/// Graphics adapter. This class wraps the functionality for a physical
/// display adapter. Graphics adapter owns all created \link
/// d3d9::graphics_device devices. \endlink
///
/// \todo make sure we can have multiple devices in single adapter.
/// \todo make sure we can have multiple monitors in single adapter.
/// \todo make sure we can have multiple monitors in multiple adapters.
///
class graphics_adapter : boost::noncopyable
{
public:
    /// Creates an adapter for physical display adapter indexed by
    /// \a adapter_index.
    graphics_adapter(graphics_system& system, UINT adapter_index);

    /// Describes this physical display adapter in the system.
    D3DADAPTER_IDENTIFIER9 identifier();

    /// Creates a device to represent the display adapter.
	std::shared_ptr<graphics_device> create_device(
        graphics_device_parameters const& parameters, HWND handle);

    D3DCAPS9 const& get_device_caps(D3DDEVTYPE device_type);

    bool check_depth_stencil_match(D3DDEVTYPE device_type,
        D3DFORMAT adapter_format, D3DFORMAT rendertarget_format,
        D3DFORMAT depth_stencil_format);

    bool check_device_format(D3DDEVTYPE device_type,
        D3DFORMAT adapter_format, DWORD usage, D3DRESOURCETYPE resource_type,
        D3DFORMAT check_format);

    bool check_device_multisample_type(D3DDEVTYPE device_type,
        D3DFORMAT surface_format, bool windowed,
        D3DMULTISAMPLE_TYPE multisample_type, DWORD& quality_levels);

    /// \return \c true if the given type is supported.
    bool check_device_type(D3DDEVTYPE type, D3DFORMAT display_format,
        D3DFORMAT backbuffer_format, bool windowed);

    /// \return \c true if this adapter has one or more devices.
    bool has_device() const;

    //shared_ptr<graphics_device> first_device() const;

    D3DPRESENT_PARAMETERS const& getPresentationParameters();

private: // helper functions
    UINT display_mode_count(D3DFORMAT format);
    bool enum_mode(D3DFORMAT format, UINT mode_index, D3DDISPLAYMODE& mode);

private: // data members
    graphics_system& m_system;
    UINT m_adapter_index;
    D3DCAPS9 m_caps;
    D3DPRESENT_PARAMETERS m_present_parameters;
	std::vector<std::shared_ptr<graphics_device> > m_devices;
};

}} // namespace pt::d3d9

#endif
