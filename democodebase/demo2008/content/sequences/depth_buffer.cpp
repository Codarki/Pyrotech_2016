#include "depth_buffer.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/config_node.h"
#include "pt/animatable_value.h"

#include "../utility.h"
#include "pt/ui/common.h"
#include "pt/math_utils.h"
#include "pt/parameter_node.h"
#include "pt/vector_utility.h"
#include "pt/any_error.h"

namespace content { namespace sequences {

depth_buffer::depth_buffer(d3d9::graphics_device& device,
    config_node const& config,parameter_collection const& parameters)
: m_basic(config,parameters)
, m_device(&device)
{
    assert(config.key() == "depth_buffer");

    pt::vector2i resolution;

    config_node::const_iterator i = config.find_child("resolution");
    if (i != config.end())
    {
        if (config.child("resolution")->value() == "backbuffer")
        {
            D3DPRESENT_PARAMETERS present_params = device.parameters();
            resolution[0] = present_params.BackBufferWidth / 2;
            resolution[1] = present_params.BackBufferHeight / 2;
        }
        else if (config.child("resolution")->value() == "half_backbuffer")
        {
            D3DPRESENT_PARAMETERS present_params = device.parameters();
            resolution[0] = present_params.BackBufferWidth / 2;
            resolution[1] = present_params.BackBufferHeight / 2;
        }
        else
        {
            throw any_error("depth_buffer invalid resolution");
        }
    }
    else
    {
        D3DVIEWPORT9 view = d3d9::viewport(device);
        resolution[0] = view.Width;
        resolution[1] = view.Height;
    }


    D3DFORMAT format = D3DFMT_D24X8;
    D3DMULTISAMPLE_TYPE multi_sample = device.parameters().MultiSampleType;
    DWORD multi_sample_quality = device.parameters().MultiSampleQuality;

    m_depth_stencil = d3d9::create_depth_stencil_surface(device,
        resolution[0], resolution[1], format, multi_sample,
        multi_sample_quality, true);

    std::shared_ptr<parameter_node> curves = *parameters.child("curves");
    m_clear_color = create_curve_value<vector3f>(
        *config.child("clear_color"), curves);
}

string_wrapper const& depth_buffer::name() const
{
    return m_basic.name();
}

std::shared_ptr<d3d9::texture_2d> depth_buffer::draw(rendertarget& target,
    float time)
{
    assert(m_depth_stencil);
        DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
        //vector4f clear_color(0, 0, 0.25f, 0);
        //vector4f clear_color(0, 0, 0, 0);

        vector4f clear_color(m_clear_color->value_at(time), 1.0f);
        clear_color[0] = pt::clamp(clear_color[0], 0.0f, 1.0f);
        clear_color[1] = pt::clamp(clear_color[1], 0.0f, 1.0f);
        clear_color[2] = pt::clamp(clear_color[2], 0.0f, 1.0f);
        clear_color[3] = pt::clamp(clear_color[3], 0.0f, 1.0f);

    //d3d9::clear(*m_device, flags, clear_color, 1.0f, 0);

    d3d9::scoped_depth_stencil depth(*m_device, m_depth_stencil);

    //d3d9::clear(*m_device,  D3DCLEAR_ZBUFFER, clear_color, 1.0f, 0);
    d3d9::clear(*m_device,  flags, clear_color, 1.0f, 0);
    
    d3d9::scoped_render_state culling(*m_device, D3DRS_CULLMODE, D3DCULL_CCW);
    d3d9::scoped_render_state zenable(*m_device, D3DRS_ZENABLE, TRUE);
    
    return m_basic.draw(target, time);
}

float depth_buffer::start_time() const
{
    return m_basic.start_time();
}

float depth_buffer::end_time() const
{
    return m_basic.end_time();
}

void depth_buffer::set_override_view(d3d9::rendering::item* view)
{
    m_basic.set_override_view(view);
}

void depth_buffer::link_inputs(composite_sequence_collection const& sequences)
{
    m_basic.link_inputs(sequences);
}

}} // namespace content::sequences
