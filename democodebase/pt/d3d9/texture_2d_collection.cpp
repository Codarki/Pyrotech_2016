#include "texture_2d_collection.h"


namespace pt { namespace d3d9 {

namespace {
	std::unique_ptr<texture_2d_collection> instance;
	com_ptr<IDirect3DSurface9> g_depth_stencil;
    bool cleaned_up = false;
}

texture_2d_collection& default_texture_2d_collection()
{
    assert(!cleaned_up);
    if (!instance)
        instance = std::make_unique<texture_2d_collection>();
    return *instance;
}

void clear_default_texture_2d_collection()
{
    if (instance)
    {
        instance->clear();
        instance.reset();
    }
    cleaned_up = true;
}

com_ptr<IDirect3DSurface9> get_depth_stencil(d3d9::graphics_device& device, pt::vector2i resolution)
{
	if (g_depth_stencil == NULL)
	{
		D3DFORMAT format = D3DFMT_D24X8;
		D3DMULTISAMPLE_TYPE multi_sample = device.parameters().MultiSampleType;
		DWORD multi_sample_quality = device.parameters().MultiSampleQuality;

		g_depth_stencil = d3d9::create_depth_stencil_surface(device,
			resolution[0], resolution[1], format, multi_sample,
			multi_sample_quality, true);
	}

	return g_depth_stencil;
}

void clear_default_depth_stencil() 
{
    g_depth_stencil.reset();
}

}} // namespace pt::d3d9

