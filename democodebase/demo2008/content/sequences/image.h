#ifndef CONTENT_SEQUENCES_IMAGE_H
#define CONTENT_SEQUENCES_IMAGE_H

#include "simple_composite_sequence.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/path_fwd.h"

namespace content { namespace sequences {

class image : public simple_composite_sequence
{
public:
    image(d3d9::graphics_device& device, std::string const& name,
        pt::path const& path);
    image(d3d9::graphics_device& device, config_node const& config);

    std::shared_ptr<d3d9::texture_2d const> texture() const;

private: // simple_composite_sequence virtual function overloads
    std::shared_ptr<d3d9::texture_2d> on_draw(rendertarget& target,
        float time, std::shared_ptr<d3d9::texture_2d> child_result);

private: // data members
    std::shared_ptr<d3d9::texture_2d> m_texture;
    std::string m_name;
};

}} // namespace content::sequences

#endif
