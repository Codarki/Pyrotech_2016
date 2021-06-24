#include "shading_program.h"
#include "variable_owner.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"

namespace pt { namespace d3d9 { namespace rendering {

shading_program::shading_program(graphics_device& device,
    std::shared_ptr<pixel_shader> pixel_shader,
    std::shared_ptr<vertex_shader> vertex_shader)
: m_device(&device)
, m_pixel_shader(pixel_shader)
, m_vertex_shader(vertex_shader)
{
}

void shading_program::use()
{
    if (m_pixel_shader)
    {
        m_pixel_shader->use();
    }
    else
    {
        set_pixel_shader(*m_device, com_ptr<IDirect3DPixelShader9>());
    }
    m_vertex_shader->use();
}

void shading_program::prepare(
    std::vector<variable_owner const*> const& owners, float time) const
{
    for (variable_owner const* owner : owners)
    {
        // Optional pixel shader.
        if (m_pixel_shader)
            owner->update_target(time, m_pixel_shader->constants());
        owner->update_target(time, m_vertex_shader->constants());
    }
}

void shading_program::create_resources(
    std::vector<variable_owner*> const& owners)
{
    //m_pixel_shader->constants().set_defaults();
    //m_vertex_shader->constants().set_defaults();

    for (variable_owner* owner : owners)
    {
        auto j = m_prepared_owners.find(owner);
        if (j != m_prepared_owners.end())
            continue;

        bool result = false;

        // Optional pixel shader.
        if (m_pixel_shader && m_pixel_shader->constants().count() > 0)
            result = owner->try_create_constants(m_pixel_shader->constants());

        if (m_vertex_shader->constants().count() > 0)
            result = owner->try_create_constants(m_vertex_shader->constants());

        result;

        m_prepared_owners.insert(owner);
    }
}

}}} // namespace pt::d3d9::rendering
