#pragma once
#ifndef PT_D3D9_RENDERING_VARIABLE_SETTER_H
#define PT_D3D9_RENDERING_VARIABLE_SETTER_H

namespace pt { namespace d3d9 { namespace rendering {

/// Variable setter is responsible for providing setting mechanism for hlsl
/// shader constants, samplers and textures.
class variable_setter
{
public:
    virtual ~variable_setter() {}

public:
    virtual void update_target(float time) const = 0;
};

}}} // namespace pt::d3d9::rendering

#endif
