#ifndef PT_D3D9_TEXTURE_2D_SOURCE_H
#define PT_D3D9_TEXTURE_2D_SOURCE_H

#include "texture_2d_source_base.h"
#include "pt/math/step_curve.h"
#include "texture_2d_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {

/// Data set where values are shared pointers to texture_2d.
class texture_2d_data_set : public pt::math::data_set<std::shared_ptr<texture_2d>>
{
};

/// Step curve with texture_2d values.
class texture_2d_step_curve : public pt::math::step_curve<texture_2d_data_set>
{
public:
    texture_2d_step_curve(texture_2d_data_set const& data)
    : step_curve(data)
    {
    }
};


/// animatable_value_source which returns texture_2d objects.
class texture_2d_source : public texture_2d_source_base
{
public:
    texture_2d_source(texture_2d_step_curve const& curve);

public: // interface required by animatable_value_source<shared_ptr<texture_2d> >
    virtual bool is_constant() const;
	virtual std::shared_ptr<texture_2d> value_at(float time) const;
    virtual string_wrapper type_name() const;

private: // data members
    texture_2d_step_curve m_curve;
};

}} // namespace pt::d3d9

#endif
