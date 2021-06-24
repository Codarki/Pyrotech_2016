#include "sampler_setter.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/texture_2d_value.h"

namespace pt { namespace d3d9 { namespace rendering {

sampler_setter::sampler_setter(sampler_state const& sampler,
    std::shared_ptr<texture_2d_value> texture)
:   m_sampler(sampler)
,   m_texture(texture)
{
}

void sampler_setter::update_target(float time) const
{
    m_sampler.use();
    std::shared_ptr<d3d9::texture_2d> tex = m_texture->value_at(time);

    set_texture(m_sampler.mutable_device(),
        static_cast<DWORD>(m_sampler.index()), tex->native());
}

}}} // namespace pt::d3d9::rendering
