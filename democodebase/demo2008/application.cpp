#include "application.h"
#include "content/demo_player.h"
#include "content/loading_screen.h"
#include "content/user_abort.h"
#include "pt/any_error.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/uncompressed_archive.h"
#include "pt/scoped_assignment.h"
#include "pt/d3d9/graphics_adapter.h"
#include "pt/ui/elements/window.h"
#include "pt/ui/native_window.h"
#include "content/utility.h"

namespace content {

vector2i application::m_resolution;

void check_support(d3d9::graphics_adapter& adapter)
{
    D3DDEVTYPE device_type = D3DDEVTYPE_HAL;
    D3DFORMAT adapter_format = D3DFMT_X8R8G8B8;
    D3DFORMAT backbuffer_format = D3DFMT_X8R8G8B8;
    
    log(format("Num. of simultaneous RTs = \"%1%\"") % adapter.get_device_caps(device_type).NumSimultaneousRTs);
    /*
    TODO: Add check for simultaneous rendertargets == 3, if we are implementing SSAO for multiple render targets
    */
    if (adapter.get_device_caps(device_type).PixelShaderVersion
        < D3DPS_VERSION(3,0))
    {
        throw any_error("Pixel shader 3.0 required");
    }

    if (!adapter.check_device_format(device_type, adapter_format,
        D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3DRTYPE_TEXTURE,
        backbuffer_format))
    {
        throw any_error("backbuffer format must support alpha blending");
    }

    if (!adapter.check_device_format(device_type, adapter_format,
        D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
        D3DRTYPE_TEXTURE, D3DFMT_A16B16G16R16F))
    {
        throw any_error("need to support D3DFMT_A16B16G16R16F render target");
    }

    if (!adapter.check_device_format(device_type, adapter_format,
        D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
        D3DRTYPE_SURFACE, backbuffer_format))
    {
        throw any_error("Need to support post-pixel processing");
    }
}

D3DFORMAT check_luminance_support(d3d9::graphics_adapter& adapter)
{
    D3DDEVTYPE device_type = D3DDEVTYPE_HAL;
    D3DFORMAT adapter_format = D3DFMT_X8R8G8B8;

    if (!adapter.check_device_format(device_type, adapter_format,
        D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_R32F))
    {
        if (!adapter.check_device_format(device_type, adapter_format,
            D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_R16F))
        {
            throw any_error("need to support D3DFMT_R32F or D3DFMT_R16F render target");
        }
        return D3DFMT_R16F;
    }
    return D3DFMT_R32F;
}

bool is_depth_stencil_supported(d3d9::graphics_adapter& adapter,
    D3DFORMAT depth_stencil_format)
{
    D3DDEVTYPE device_type = D3DDEVTYPE_HAL;
    D3DFORMAT adapter_format = D3DFMT_X8R8G8B8;
    D3DFORMAT rendertarget_format = D3DFMT_A16B16G16R16F;

    if (adapter.check_device_format(device_type, adapter_format,
        D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depth_stencil_format))
    {
        return adapter.check_depth_stencil_match(device_type,
            adapter_format, rendertarget_format, depth_stencil_format);
    }
    return false;
}

void check_depth_stencil_support(d3d9::graphics_adapter& adapter)
{
    bool d16_supported = is_depth_stencil_supported(adapter, D3DFMT_D16);
    bool d32_supported = is_depth_stencil_supported(adapter, D3DFMT_D32);
    bool d24x8_supported = is_depth_stencil_supported(adapter, D3DFMT_D24X8);

    if (!(d16_supported || d32_supported || d24x8_supported))
        throw any_error("No depth stencil supported");
}

application::application(command_line const& commands, std::string demoname)
:   m_commands(commands)
,   m_window(demoname)
,   m_full_screen(!m_commands.has("window"))
{
    m_resolution = vector2i(1024, 768);
    if (m_commands.has("res"))
    {
        std::string res_text = m_commands.value_of("res");
        std::string::size_type pos = res_text.find('x');
        if (pos != std::string::npos)
        {
            m_resolution[0] = lexical_cast<int>(res_text.substr(0, pos));
            m_resolution[1] = lexical_cast<int>(res_text.substr(pos + 1));
        }
    }

    log(format("Window resolution: \"%1%\"x\"%2%\"") % m_resolution[0] % m_resolution[1]);

    if (m_full_screen)
    {
        m_window.create(demoname, m_resolution[0], m_resolution[1]);
    }
    else
    {
        m_window.create_with_client_size(
            demoname, m_resolution[0], m_resolution[1]);
    }
}

application::~application()
{
    m_player.reset();
}

namespace {
    D3DFORMAT g_luminance_format;

    int msaa_count_from_command_line(
        command_line& commands)
    {
        if (commands.has("aa"))
        {
            std::string msaa_value_text = commands.value_of("aa");
            int msaa_value = lexical_cast<int>(msaa_value_text);

            if (msaa_value == 1)
                return 0;

            return msaa_value;
        }
        return 0;
    }
}

void application::check_requirements()
{
    m_window.init_graphics_system();

    d3d9::graphics_adapter& adapter = *m_window.graphics_adapter();

    check_support(adapter);
    g_luminance_format = check_luminance_support(adapter);
    check_depth_stencil_support(adapter);

    if (m_commands.has("aa"))
    {
        std::string msaa_value_text = m_commands.value_of("aa");
        int msaa_value = lexical_cast<int>(msaa_value_text);

        D3DMULTISAMPLE_TYPE type = static_cast<D3DMULTISAMPLE_TYPE>(
            D3DMULTISAMPLE_NONE + msaa_value);
        D3DFORMAT surface_format = D3DFMT_A16B16G16R16F;

        DWORD quality_levels;

        if (!m_window.graphics_adapter()->check_device_multisample_type(
            D3DDEVTYPE_HAL, surface_format, !m_full_screen, type,
            quality_levels))
        {
            throw any_error("MSAA mode not supported");
        }
    }
}

int application::run()
{
    try
    {
        int msaa_count = msaa_count_from_command_line(m_commands);

        m_window.init_direct3d(msaa_count, m_full_screen);

        if (m_full_screen)
            ShowCursor(FALSE);

        try
        {
            if (pt::exists("demo.dat"))
                m_archive.reset(new uncompressed_archive("demo.dat"));
        }
        catch(std::exception&)
        {
        }

        
        //Create render textures which are sameside as backbuffer for MRT rendering
        //vector2i resolution = content::application::resolution();
        //d3d9::graphics_adapter& adapter = *m_window.graphics_adapter();
        //D3DPRESENT_PARAMETERS present_params = adapter.getPresentationParameters();
        //create_and_add_rendertarget_texture2d(m_window.direct3d_device(),"MRTRenderTexture1",present_params.BackBufferWidth,present_params.BackBufferHeight,8);

        // Load and warm up.
        auto loading_screen = std::make_shared<content::loading_screen>(
            m_window.direct3d_device(), &m_window, m_resolution);

        m_window.set_handler(loading_screen);

        loading_screen->fade_in();
        m_player.reset(new demo_player(
            m_window.direct3d_device(), &m_commands, *loading_screen, &m_window));

        m_player->set_resolution(m_resolution);
        m_player->warm_up();
        loading_screen->fade_out();
        loading_screen.reset();

        m_player->update();

        // Play.
        m_window.set_handler(m_player);
        m_window.invalidate();
        m_pump.reset(new win32::message_pump);
        return m_pump->run(*this);
    }
    catch(content::user_abort&)
    {
        log("User aborted.");
        return 0;
    }
    catch(std::exception& e)
    {
        // Try to close graphics and show messagebox while our window is
        // still alive.
        m_player.reset();
        m_pump.reset();
        m_window.close_direct3d();
		m_window.set_handler(nullptr);
        log(e.what());
        
        MessageBox(m_window.native()->handle(), e.what(), "Exception",
            MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

        //MessageBox(0, e.what(), "Exception",
        //    MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
        return -1;
    }
}

bool application::processes_on_idle() const
{
    return true;
}

float application::next_processing_delta_time() const
{
    return 0.0f;//1.0f / 60.0f;
}

void application::on_idle()
{
    //m_player->update();

    if (m_player->finished())
    {
        m_pump->quit();
    }
    else
    {
        m_window.invalidate();
    }
}

} // namespace content
