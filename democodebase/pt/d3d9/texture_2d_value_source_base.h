#pragma once
#ifndef PT_D3D9_TEXTURE_2D_VALUE_SOURCE_BASE_H
#define PT_D3D9_TEXTURE_2D_VALUE_SOURCE_BASE_H

#include "texture_2d_fwd.h"
#include "pt/animatable_value_source.h"
#include "pt/boost/shared_ptr.h"

namespace pt { namespace d3d9 {

/// Base for sources for texture_2d_value.
class texture_2d_value_source_base :
    public animatable_value_source<boost::shared_ptr<texture_2d> >
{
public: // interface required by animatable_value_source<shared_ptr<texture_2d> >

    //virtual bool is_constant() const = 0;
    //virtual shared_ptr<texture_2d> value_at(float time) const = 0;
    //virtual string_wrapper type_name() const = 0;
};

}} // namespace pt::d3d9

#endif
