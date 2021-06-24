#include "texture_2d_source.h"

namespace pt { namespace d3d9 {

texture_2d_source::texture_2d_source(texture_2d_step_curve const& curve)
: m_curve(curve)
{
}

bool texture_2d_source::is_constant() const
{
    return false;
}

std::shared_ptr<texture_2d> texture_2d_source::value_at(float time) const
{
    return m_curve.value_at(time);
}

string_wrapper texture_2d_source::type_name() const
{
    return "texture_2d_source";
}

}} // namespace pt::d3d9
