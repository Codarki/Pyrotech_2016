#include "rendertarget.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/texture_2d.h"

namespace content { namespace sequences {

rendertarget::rendertarget()
:   d3d9::optional_device_reference(0)
{
}

rendertarget::rendertarget(d3d9::graphics_device& device,
    std::shared_ptr<d3d9::texture_2d> texture)
:   d3d9::optional_device_reference(&device)
,   m_texture(texture)
{
}

rendertarget::rendertarget(d3d9::graphics_device& device,
    com_ptr<IDirect3DSurface9> surface)
:   d3d9::optional_device_reference(&device)
,   m_surface(surface)
{
}

std::shared_ptr<d3d9::texture_2d> rendertarget::texture()
{
    return m_texture;
}

com_ptr<IDirect3DSurface9> rendertarget::surface()
{
    return m_surface;
}

void rendertarget::activate(int index) 
{
	com_ptr<IDirect3DSurface9> surface = m_texture
		? d3d9::surface_level(*m_texture, 0) : m_surface;
	if (surface)
	{
		d3d9::set_render_target(this->device(), surface, index);
	}
}

// scoped_optional_rendertarget
scoped_optional_rendertarget::scoped_optional_rendertarget(
    rendertarget& target, int index)
:   m_target(target),m_index(index)
{
    UINT l_index = this->m_index;

    com_ptr<IDirect3DSurface9> surface = target.texture()
        ? d3d9::surface_level(*target.texture(), 0) : target.surface();
    if (surface)
    {
        m_old = d3d9::render_target(target.device(), l_index);
        d3d9::set_render_target(target.device(), surface, l_index);
    }
}

scoped_optional_rendertarget::~scoped_optional_rendertarget()
{
    if (m_old)
    {
        d3d9::set_render_target(m_target.device(), m_old, this->m_index);
    }
}

}} // namespace content::sequences
