#include "graphics_system.h"
#include "graphics_adapter.h"
#include "pt/any_error.h"
#include "pt/log.h"

// link libraries
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")

namespace pt { namespace d3d9 {
//namespace {
//    graphics_system* g_system = 0;
//} // anonymous namespace

graphics_system::graphics_system()
{
    log("Initializing graphics system");

    //if (g_system)
    //    throw any_error("graphics_system already created");
    //g_system = this;

    *m_d3d.receive() = Direct3DCreate9(D3D_SDK_VERSION);

    if (!m_d3d)
        throw any_error("Direct3DCreate9 failed.");

    UINT adapted_count = m_d3d->GetAdapterCount();

    for (UINT i = 0; i < adapted_count; ++i)
    {
		m_adapters.push_back(std::shared_ptr<graphics_adapter>(
            new graphics_adapter(*this, i)));
    }
}

graphics_system::~graphics_system()
{
    //if (!g_system)
    //    throw any_error("deleting empty graphics_system");
    //g_system = 0;
}

//bool graphics_system::has_instance()
//{
//    return g_system != 0;
//}
//
//graphics_system& graphics_system::instance()
//{
//    assert(g_system);
//    if (!g_system)
//        throw any_error("No instance of graphics_system");
//    return *g_system;
//}

//void graphics_system::create_device(size_t adapter_index, int width,
//    int height, int bits, HWND handle, bool fullscreen)
//{
//    if (adapter_index > m_adapters.size())
//        throw any_error("Invalid adapter index");
//
//    m_adapters[adapter_index]->create_device(width, height, bits, handle,
//        fullscreen);
//}

std::shared_ptr<graphics_adapter const> graphics_system::adapter(size_t index) const
{
    if (index > m_adapters.size())
        throw any_error("Invalid adapter index");

    return m_adapters[index];
}

std::shared_ptr<graphics_adapter> graphics_system::adapter(size_t index)
{
    if (index > m_adapters.size())
        throw any_error("Invalid adapter index");

    return m_adapters[index];
}

com_ptr<IDirect3D9> graphics_system::native()
{
    return m_d3d;
}

}} // namespace pt::d3d9
