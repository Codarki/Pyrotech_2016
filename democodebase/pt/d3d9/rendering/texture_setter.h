#ifndef PT_D3D9_RENDERING_TEXTURE_SETTER_H
#define PT_D3D9_RENDERING_TEXTURE_SETTER_H

#include "variable_setter.h"
#include "pt/d3d9/texture_2d_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class texture_setter : public variable_setter
{
public:
	texture_setter(std::shared_ptr<texture_2d> texture, size_t sampler_index);

public: // interface required by variable_setter
    void update_target(float time) const;

private:
    size_t m_sampler_index;
    std::shared_ptr<texture_2d> m_texture;
};

}}} // namespace pt::d3d9::rendering

#endif
