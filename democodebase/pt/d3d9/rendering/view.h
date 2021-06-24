#ifndef PT_D3D9_RENDERING_VIEW_H
#define PT_D3D9_RENDERING_VIEW_H

#include "pt/math/plane.h"
#include "pt/animatable_value.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class variable_owner;
class projection_matrix;

class view
{
public:
    virtual ~view() {}

public:
    virtual projection_matrix& projection() = 0;
    virtual variable_owner& variables() = 0;
    virtual variable_owner const& variables() const = 0;
    virtual std::shared_ptr<projection_matrix> shared_projection() const = 0;
    /// Returns clipping planes in view space (local space).
    virtual std::vector<animatable_value<math::plane>> const& clipping_planes() const = 0;
    virtual bool flip_culling() const = 0;
};

}}} // namespace pt::d3d9::rendering

#endif
