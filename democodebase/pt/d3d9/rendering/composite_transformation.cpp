#include "composite_transformation.h"

namespace pt { namespace d3d9 { namespace rendering {

void composite_transformation::push_back(
    std::shared_ptr<transformation> const& transformation)
{
    m_transformations.push_back(transformation);
}

matrix4x4f composite_transformation::get_local_to_parent_at(float time) const
{
    matrix4x4f result;

    for (auto const& i : m_transformations)
    {
        auto const& transformation = *i;
        //result *= ((*i)->local_to_parent_at(time));
        result = result * transformation.local_to_parent_at(time);
    }

    return result;
}

bool composite_transformation::get_is_constant() const
{
    for (auto const& i : m_transformations)
    {
        if (!i->is_constant())
            return false;
    }
    return true;
}

}}} // namespace pt::d3d9::rendering
