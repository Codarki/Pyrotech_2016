#include "graphics_adapter.h"
#include "graphics_device.h"
#include "graphics_system.h"
#include "error.h"
#include "pt/any_error.h"
#include "pt/debug_only.h"
#include "pt/log.h"
#include "pt/string_wrapper.h"
#include "pt/boost/format.h"
#include <limits>

namespace pt { namespace d3d9 {

// graphics_device_parameters

graphics_device_parameters::graphics_device_parameters()
:   width(1024)
,   height(768)
,   bits(32)
,   refresh_rate(0)
,   aa_count(1)
,   aa_quality(0)
,   fullscreen(false)
,   swap(swap_discard)
{
}

// graphics_adapter

graphics_adapter::graphics_adapter(graphics_system& system,
    UINT adapter_index)
:   m_system(system)
,   m_adapter_index(adapter_index)
{
    log("Created graphics adapter");
}

D3DADAPTER_IDENTIFIER9 graphics_adapter::identifier()
{
    D3DADAPTER_IDENTIFIER9 identifier;
    DWORD flags = 0;
    check_dx(m_system.native()->GetAdapterIdentifier(m_adapter_index,
        flags, &identifier),
        "IDirect3D9::GetAdapterIdentifier");
    return identifier;
}

namespace {
    D3DMULTISAMPLE_TYPE get_ms_type(int aa_count)
    {
        if (aa_count == 1)
            return D3DMULTISAMPLE_NONE;

        return D3DMULTISAMPLE_TYPE(aa_count);
    }

    D3DPRESENT_PARAMETERS create_d3d_present_params(
        graphics_device_parameters const& parameters)
    {
		if (parameters.aa_count)
		{
		}
		log((boost::format("MSAA count: %1%")
                        % parameters.aa_count).str());
        D3DPRESENT_PARAMETERS params;
        params.BackBufferCount = 1;
        params.MultiSampleType = get_ms_type(parameters.aa_count);//D3DMULTISAMPLE_NONE;//
        params.MultiSampleQuality = 0;
        params.EnableAutoDepthStencil = false;
        params.AutoDepthStencilFormat = D3DFMT_D24X8;
        params.Flags = 0;
        //params.Flags |= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
        // works only on 2000/XP
        //params.Flags |= D3DPRESENTFLAG_DEVICECLIP;
        params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        //params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        //params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        return params;
    }
} // anonymous namespace

//shared_ptr<graphics_device> graphics_adapter::create_device(int width,
//    int height, int bits, HWND handle, bool fullscreen, swapeffect swap)

std::shared_ptr<graphics_device> graphics_adapter::create_device(
    graphics_device_parameters const& parameters_asd, HWND handle)
{
    graphics_device_parameters parameters = parameters_asd;
    D3DFORMAT format = D3DFMT_X8R8G8B8;

    // \todo refresh rate. enumerations will return some crazy modes, up to
    // 200hz, so we cant just pick the highest. And picking lowest isnt cool
    // either. There should be default refresh rates somewhere, in registry
    // maybe. Win32 API for monitors is rather confusing though.
    //
    // We stick to 60hz in fullscreen and desktop hz in windowed for now.
    //UINT refresh_rate = parameters.refresh_rate;

    if (parameters.fullscreen)
    {
        bool found_mode = false;
        if (parameters.bits == 32)
        {
            // \todo loop through all formats, or make this configurable.
            // D3DFMT_X8R8G8B8
            // D3DFMT_A8R8G8B8
            // D3DFMT_A2R10G10B10
            format = D3DFMT_X8R8G8B8;
        }
        else if (parameters.bits == 16)
        {
            // \todo loop through all formats, or make this configurable.
            // D3DFMT_A1R5G5B5
            // D3DFMT_R5G6B5
            // D3DFMT_X1R5G5B5
            format = D3DFMT_R5G6B5;
        }

        if (parameters.refresh_rate == 0)
            parameters.refresh_rate = 60;

        // Enumerate the modes and look for a match.
        UINT closest_refresh_rate = 0;
        UINT min_difference_in_hz = UINT_MAX;

        UINT count = display_mode_count(format);
        for (UINT i = 0; i < count; ++i)
        {
            D3DDISPLAYMODE mode;
            if (enum_mode(format, i, mode))
            {

                if (parameters.width == static_cast<int>(mode.Width)
                    && parameters.height == static_cast<int>(mode.Height)
                    && parameters.refresh_rate == static_cast<int>(mode.RefreshRate))
                {

                    found_mode = true;
                } 
                else if (parameters.width == static_cast<int>(mode.Width)
                    && parameters.height == static_cast<int>(mode.Height))
                {
                    UINT diff = std::abs(60 - static_cast<int>(mode.RefreshRate));
                    if (diff < min_difference_in_hz)
                    {
                        min_difference_in_hz = diff;
                        closest_refresh_rate = static_cast<int>(mode.RefreshRate);
                    }
                    found_mode = true;
                }

            }
        }
        if (!found_mode)
        {
            throw any_error((boost::format(
                "Could not enumerate display mode %1% x %2% %3%hz")
                % parameters.width
                % parameters.height
                % parameters.refresh_rate).str());
        }
        if (closest_refresh_rate > 0)
            parameters.refresh_rate = closest_refresh_rate;

        log((boost::format("Enumerated mode %1% x %2% %3%hz")
                        % parameters.width
                        % parameters.height
                        % parameters.refresh_rate).str());
    }

    //HMONITOR monitor_handle =
    //    graphics_system::instance().native()->GetAdapterMonitor(
    //    m_adapter_index);
    //assert(monitor_handle);

    //std::string monitor_name = enum_monitor_name_for_display_adapter(
    //    m_adapter_index);

    //DEVMODE dev_mode;
    //dev_mode.dmSize = sizeof(DEVMODE);
    //DWORD monitor_mode_num = 0;
    //while(EnumDisplaySettings(monitor_name.c_str(), monitor_mode_num,
    //    &dev_mode) != 0)
    //{
    //    log(boost::format("Monitor mode %1%: %2% x %3% %4% bits %5%hz")
    //        % monitor_mode_num
    //        % dev_mode.dmPelsWidth
    //        % dev_mode.dmPelsHeight
    //        % dev_mode.dmBitsPerPel
    //        % dev_mode.dmDisplayFrequency);
    //    monitor_mode_num++;
    //    dev_mode.dmSize = sizeof(DEVMODE);
    //}

    // Create device.

    
	m_present_parameters
        = create_d3d_present_params(parameters);
    m_present_parameters.BackBufferWidth = parameters.width;
    m_present_parameters.BackBufferHeight = parameters.height;
    m_present_parameters.BackBufferFormat = format;
    m_present_parameters.hDeviceWindow = handle;
    m_present_parameters.Windowed = !parameters.fullscreen;

	log(str(boost::format("Backbuffer dimensions: %1%x%2%")
        % m_present_parameters.BackBufferWidth % m_present_parameters.BackBufferHeight));

    switch(parameters.swap)
    {
    case swap_copy:
        m_present_parameters.SwapEffect = D3DSWAPEFFECT_COPY;
        break;

    case swap_discard:
        m_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        break;

    case swap_flip:
        m_present_parameters.SwapEffect = D3DSWAPEFFECT_FLIP;
        break;

    default:
        throw any_error("invalid swap effect");
    }

    D3DFORMAT display_format = format;
    D3DFORMAT backbuffer_format = format;

    PT_DEBUG_ONLY(bool hal_supported = )
        check_device_type(D3DDEVTYPE_HAL, display_format, backbuffer_format,
        !parameters.fullscreen);

    assert(hal_supported);

	std::shared_ptr<graphics_device> new_device(new graphics_device(m_system,
        m_present_parameters, m_adapter_index, handle));

    m_devices.push_back(new_device);
    return new_device;
}

D3DCAPS9 const& graphics_adapter::get_device_caps(D3DDEVTYPE device_type)
{
    check_dx(m_system.native()->GetDeviceCaps(m_adapter_index,
        device_type, &m_caps), "IDirect3D9::GetDeviceCaps");
    return m_caps;
}

bool graphics_adapter::check_depth_stencil_match(D3DDEVTYPE device_type,
    D3DFORMAT adapter_format, D3DFORMAT rendertarget_format,
    D3DFORMAT depth_stencil_format)
{
    HRESULT result = m_system.native()->CheckDepthStencilMatch(m_adapter_index,
        device_type, adapter_format, rendertarget_format, depth_stencil_format);

    if (result == D3D_OK)
        return true;
    else if (result == D3DERR_NOTAVAILABLE)
        return false;

    check_dx(result, "IDirect3D9::CheckDepthStencilMatch");
    return false;
}

bool graphics_adapter::check_device_format(D3DDEVTYPE device_type,
    D3DFORMAT adapter_format, DWORD usage, D3DRESOURCETYPE resource_type,
    D3DFORMAT check_format)
{
    HRESULT result = m_system.native()->CheckDeviceFormat(m_adapter_index,
        device_type, adapter_format, usage, resource_type, check_format);

    if (result == D3D_OK)
        return true;
    else if (result == D3DERR_NOTAVAILABLE)
        return false;

    check_dx(result, "IDirect3D9::CheckDeviceFormat");
    return false;
}

bool graphics_adapter::check_device_multisample_type(D3DDEVTYPE device_type,
    D3DFORMAT surface_format, bool windowed,
    D3DMULTISAMPLE_TYPE multisample_type, DWORD& quality_levels)
{
    HRESULT result = m_system.native()->CheckDeviceMultiSampleType(
        m_adapter_index, device_type, surface_format, windowed,
        multisample_type, &quality_levels);

    if (result == D3D_OK)
        return true;
    else if (result == D3DERR_NOTAVAILABLE)
        return false;

    check_dx(result, "IDirect3D9::CheckDeviceMultiSampleType");
    return false;
}

bool graphics_adapter::check_device_type(D3DDEVTYPE type,
    D3DFORMAT display_format, D3DFORMAT backbuffer_format, bool windowed)
{
    HRESULT result = m_system.native()->CheckDeviceType(m_adapter_index,
        type, display_format, backbuffer_format, windowed);

    if (result == D3D_OK)
        return true;
    else if (result == D3DERR_NOTAVAILABLE)
        return false;

    check_dx(result, "IDirect3D9::CheckDeviceType");
    return false;
}

bool graphics_adapter::has_device() const
{
    return !m_devices.empty();
}

//shared_ptr<graphics_device> graphics_adapter::first_device() const
//{
//    assert(has_device());
//    return m_devices[0];
//}

UINT graphics_adapter::display_mode_count(D3DFORMAT format)
{
    return m_system.native()->GetAdapterModeCount(m_adapter_index, format);
}

bool graphics_adapter::enum_mode(D3DFORMAT format, UINT mode_index,
    D3DDISPLAYMODE& mode)
{
    HRESULT result = m_system.native()->EnumAdapterModes(m_adapter_index,
        format, mode_index, &mode);

    if (result == D3D_OK)
        return true;
    else if (result == D3DERR_NOTAVAILABLE)
        return false;

    check_dx(result, "IDirect3D9::EnumAdapterModes");
    return false;
}

D3DPRESENT_PARAMETERS const& graphics_adapter::getPresentationParameters()
{
	return m_present_parameters;
}

}} // namespace pt::d3d9
