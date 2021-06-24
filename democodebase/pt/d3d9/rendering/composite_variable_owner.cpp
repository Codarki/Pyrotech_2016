#include "composite_variable_owner.h"

namespace pt { namespace d3d9 { namespace rendering {

void composite_variable_owner::push_back(std::shared_ptr<variable_owner> owner)
{
    m_owners.push_back(owner);
}

bool composite_variable_owner::try_create_constants(
    constant_buffer const& buffer)
{
    for (std::shared_ptr<variable_owner> const& owner : m_owners)
    {
        if (owner->try_create_constants(buffer))
            return true;
    }
    return false;
}

void composite_variable_owner::update_target(float time,
    constant_buffer const& buffer) const
{
    for (std::shared_ptr<variable_owner> const& owner : m_owners)
        owner->update_target(time, buffer);
}

}}} // namespace pt::d3d9::rendering
