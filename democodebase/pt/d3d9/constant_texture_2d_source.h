#ifndef PT_D3D9_CONSTANT_TEXTURE_2D_SOURCE_H
#define PT_D3D9_CONSTANT_TEXTURE_2D_SOURCE_H

#include "texture_2d_source_base.h"
#include "texture_2d_fwd.h"

namespace pt { namespace d3d9 {

/// Source for texture_2d_value.
class constant_texture_2d_source : public texture_2d_source_base
{
public:
    constant_texture_2d_source();
    constant_texture_2d_source(std::shared_ptr<texture_2d> value);

    void set_texture(std::shared_ptr<texture_2d> texture);

public: // interface required by animatable_value_source<shared_ptr<texture_2d> >
    virtual bool is_constant() const;
	virtual std::shared_ptr<texture_2d> value_at(float) const;
    virtual string_wrapper type_name() const;

private: // data members
    std::shared_ptr<texture_2d> m_value;
};

}} // namespace pt::d3d9

#endif
