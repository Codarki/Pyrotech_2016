#ifndef PT_D3D9_RENDERING_LIGHT_UTILS_H
#define PT_D3D9_RENDERING_LIGHT_UTILS_H

#include "simple_variable_owner.h"
#include "constant_setter_utility.h"
#include "transformer.h"
#include "matrix4x4f_setters.h"
#include "vector3f_setters.h"

namespace pt { namespace d3d9 { namespace rendering {

class transformer;

class light_variables : public simple_variable_owner
{
public:
    light_variables(
        transformation const& transformation,
        vector3f const& color, float const& range, 
        float const& hotspot, float const& falloff, bool const & isSpot, float const & shadow_bias)
    : m_transformation(transformation)
    , m_color(color)
    , m_range(range)
    , m_hotspot(hotspot)
    , m_falloff(falloff)
    , m_isSpot(isSpot)
    , m_shadow_bias(shadow_bias)
    {
    }

protected: // interface required by simple_variable_owner
    std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const
    {
        if (constant.name() == "light_to_world")
        {
            // TODO: Just give transformer? "light_to_world", "world_to_light"
            return std::make_unique<local_to_world_setter>(m_transformation);
        }
        if (constant.name() == "world_to_light")
        {
            return std::make_unique<world_to_local_setter>(m_transformation);
        }
        if (constant.name() == "light_position_in_world")
        {
            return std::make_unique<light_in_world_setter>(m_transformation);
        }
        if (constant.name() == "light_direction_in_world")
        {
            return std::make_unique<light_dir_in_world_setter>(m_transformation);
        }
        if (constant.name() == "light_color")
        {
            return create_unique_reference_setter<vector3f>(m_color, constant);
        }
        if(constant.name() == "shadow_bias")
        {
            return create_unique_reference_setter<float>(m_shadow_bias, constant);
        }

        //if (constant.name() == "light_range")
        //{
        //    return create_unique_reference_setter<float>(m_range, constant);
        //}
        //if(constant.name() == "light_hotspot")
        //{
        //    return create_unique_reference_setter<float>(m_hotspot, constant);
        //}
        //if(constant.name() == "light_falloff")
        //{
        //    return create_unique_reference_setter<float>(m_falloff, constant);
        //}
        //if(constant.name() == "light_isspot")
        //{
        //    return create_unique_reference_setter<bool>(m_isSpot, constant);
        //}
        return nullptr;
    }

private: // data members
    transformation const& m_transformation;
    vector3f const& m_color;
    float const& m_range;
    float const& m_hotspot;
    float const& m_falloff;
    bool const& m_isSpot;
    float const& m_shadow_bias;
};

}}} // namespace pt::d3d9::rendering

#endif
