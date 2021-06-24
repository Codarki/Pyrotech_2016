#include "sampler_state.h"
#include "device_convenience.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/string_wrapper.h"

namespace pt { namespace d3d9 {

sampler_state::sampler_state(graphics_device& device, size_t index)
:   device_reference(device)
,   m_index(index)
,   m_address_u(D3DTADDRESS_WRAP)
,   m_address_v(D3DTADDRESS_WRAP)
,   m_address_w(D3DTADDRESS_WRAP)
,   m_border_color(0)
,   m_mag_filter(D3DTEXF_POINT)
,   m_min_filter(D3DTEXF_POINT)
,   m_mip_filter(D3DTEXF_NONE)
,   m_mipmap_lod_bias(0)
,   m_max_miplevel(0)
,   m_max_anisotropy(1)
,   m_srgb_gamma(0)
,   m_element_index(0)
,   m_displacement_offset(0)
{
}

void sampler_state::use() const
{
    graphics_device& d = mutable_device();
    DWORD s = static_cast<DWORD>(m_index);

    set_sampler_state(d, s, D3DSAMP_ADDRESSU, m_address_u);
    set_sampler_state(d, s, D3DSAMP_ADDRESSV, m_address_v);
    set_sampler_state(d, s, D3DSAMP_ADDRESSW, m_address_w);
    set_sampler_state(d, s, D3DSAMP_BORDERCOLOR, m_border_color);
    set_sampler_state(d, s, D3DSAMP_MAGFILTER, m_mag_filter);
    set_sampler_state(d, s, D3DSAMP_MINFILTER, m_min_filter);
    set_sampler_state(d, s, D3DSAMP_MIPFILTER, m_mip_filter);
    set_sampler_state(d, s, D3DSAMP_MIPMAPLODBIAS, m_mipmap_lod_bias);
    set_sampler_state(d, s, D3DSAMP_MAXMIPLEVEL, m_max_miplevel);
    set_sampler_state(d, s, D3DSAMP_MAXANISOTROPY, m_max_anisotropy);
    set_sampler_state(d, s, D3DSAMP_SRGBTEXTURE, m_srgb_gamma);
    set_sampler_state(d, s, D3DSAMP_ELEMENTINDEX, m_element_index);
    set_sampler_state(d, s, D3DSAMP_DMAPOFFSET, m_displacement_offset);
}

size_t sampler_state::index() const
{
    return m_index;
}

void sampler_state::parse(config_node const& config)
{
    assert(config.key() == "sampler");

    config.parse_optional_child("address_u", m_address_u);
    config.parse_optional_child("address_v", m_address_v);
    config.parse_optional_child("address_w", m_address_w);
    config.parse_optional_child("min_filter", m_min_filter);
    config.parse_optional_child("mag_filter", m_mag_filter);
    config.parse_optional_child("mip_filter", m_mip_filter);
}

}} // namespace pt::d3d9

namespace pt {
void parse_from_text(D3DTEXTUREADDRESS& value, std::string const& text)
{
    if (text == "WRAP")
        value = D3DTADDRESS_WRAP;
    else if (text == "MIRROR")
        value = D3DTADDRESS_MIRROR;
    else if (text == "CLAMP")
        value = D3DTADDRESS_CLAMP;
    else if (text == "BORDER")
        value = D3DTADDRESS_BORDER;
    else if (text == "MIRRORONCE")
        value = D3DTADDRESS_MIRRORONCE;
    else
        //throw any_error(format("invalid D3DTEXTUREADDRESS: %1%") % text);
        throw any_error("invalid D3DTEXTUREADDRESS: " +  text);
}

void parse_from_text(D3DTEXTUREFILTERTYPE& value, std::string const& text)
{
    if (text == "NONE")
        value = D3DTEXF_NONE;
    else if (text == "POINT")
        value = D3DTEXF_POINT;
    else if (text == "LINEAR")
        value = D3DTEXF_LINEAR;
    else if (text == "ANISOTROPIC")
        value = D3DTEXF_ANISOTROPIC;
    else
        throw any_error("invalid D3DTEXTUREFILTERTYPE: " + text);
}

} // namespace pt
