#ifndef PT_D3D9_SAMPLER_STATE_H
#define PT_D3D9_SAMPLER_STATE_H

#include "device_child.h"
#include "native.h"
#include "pt/config_node_fwd.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {

class sampler_state : public device_reference
{
public:
    sampler_state(graphics_device& device, size_t index);

    void use() const;
    size_t index() const;

    void parse(config_node const& config);

private: // data members
    size_t m_index;
    D3DTEXTUREADDRESS m_address_u;
    D3DTEXTUREADDRESS m_address_v;
    D3DTEXTUREADDRESS m_address_w;
    D3DCOLOR m_border_color;
    D3DTEXTUREFILTERTYPE m_mag_filter;
    D3DTEXTUREFILTERTYPE m_min_filter;
    D3DTEXTUREFILTERTYPE m_mip_filter;
    DWORD m_mipmap_lod_bias;
    DWORD m_max_miplevel;
    DWORD m_max_anisotropy;
    DWORD m_srgb_gamma;
    DWORD m_element_index;
    DWORD m_displacement_offset;
};

}} // namespace pt::d3d9

// append into pt namespace, so config_node will use these..
namespace pt {
void parse_from_text(D3DTEXTUREADDRESS& value, std::string const& text);
void parse_from_text(D3DTEXTUREFILTERTYPE& value, std::string const& text);
}

#endif
