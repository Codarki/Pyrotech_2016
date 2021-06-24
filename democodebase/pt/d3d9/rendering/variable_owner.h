#ifndef PT_D3D9_RENDERING_VARIABLE_OWNER_H
#define PT_D3D9_RENDERING_VARIABLE_OWNER_H

#include "variable_owner_fwd.h"
#include "pt/d3d9/constant_buffer_fwd.h"

namespace pt { namespace d3d9 { namespace rendering {

/// Variable owner is responsible for setting hlsl shader constants, samplers
/// and textures. There can be multiple constant owners pointing at the same
/// constant buffer, thus components can update the constant for only those
/// it has control of.
class variable_owner
{
public:
    virtual ~variable_owner() {}

public: // interface which subclasses must implement
    virtual bool try_create_constants(constant_buffer const& buffer) = 0;
    virtual void update_target(float time,
        constant_buffer const& buffer) const = 0;
};

}}} // namespace pt::d3d9::rendering

#endif
