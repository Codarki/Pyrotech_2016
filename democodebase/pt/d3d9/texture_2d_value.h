#ifndef PT_D3D9_TEXTURE_2D_VALUE_H
#define PT_D3D9_TEXTURE_2D_VALUE_H

#include "texture_2d_source_base.h"
#include "texture_2d_fwd.h"
#include "pt/animatable_value.h"

namespace pt { namespace d3d9 {

/// Texture value which can be used in parameter_node.
class texture_2d_value : public animatable_value<std::shared_ptr<texture_2d> >
{
public:
    texture_2d_value(std::shared_ptr<texture_2d_source_base>&& source)
    : animatable_value(std::move(source))
    {
    }

    texture_2d_value(std::shared_ptr<texture_2d_source_base> const& source)
    : animatable_value(source)
    {
    }

    std::shared_ptr<texture_2d_source_base> texture_source()
    {
        return std::static_pointer_cast<texture_2d_source_base>(source());
    }
};

}} // namespace pt::d3d9

#endif
