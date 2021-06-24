#ifndef PT_D3D9_RENDERING_SAMPLER_SETTER_H
#define PT_D3D9_RENDERING_SAMPLER_SETTER_H

#include "variable_setter.h"
#include "pt/d3d9/sampler_state.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
class texture_2d_value;
}}

namespace pt { namespace d3d9 { namespace rendering {

class sampler_setter : public variable_setter
{
public:
    sampler_setter(sampler_state const& sampler,
        std::shared_ptr<texture_2d_value> texture);

public: // interface required by variable_setter
    void update_target(float time) const;

private:
    sampler_state m_sampler;
    std::shared_ptr<texture_2d_value> m_texture;
};

}}} // namespace pt::d3d9::rendering

//#include "constant_setter.inl"

#endif
