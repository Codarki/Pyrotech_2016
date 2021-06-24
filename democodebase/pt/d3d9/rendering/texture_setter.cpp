#include "texture_setter.h"
#include "../device_convenience.h"
#include "../texture_2d.h"

namespace pt { namespace d3d9 { namespace rendering {

    texture_setter::texture_setter(std::shared_ptr<texture_2d> texture,
    size_t sampler_index)
:   m_texture(texture)
,   m_sampler_index(sampler_index)
{
}

void texture_setter::update_target(float) const
{
    graphics_device& device = m_texture->mutable_device();

    set_texture(device, static_cast<DWORD>(m_sampler_index),
        m_texture->native());
}

}}} // namespace pt::d3d9::rendering
