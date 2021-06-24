#include "transformation.h"

namespace pt { namespace d3d9 { namespace rendering {

transformation::transformation()
:   m_local_to_parent_time(-666.0f)
,   m_local_to_world_time(-666.0f)
{
}

transformation::~transformation()
{
}

void transformation::set_parent(std::shared_ptr<transformation const> parent)
{
    m_parent = parent;
}

std::shared_ptr<transformation const> const& transformation::parent() const
{
    return m_parent;
}

matrix4x4f const& transformation::local_to_world_at(float time) const
{
    if (m_parent)
    {
        if (time != m_local_to_world_time)
        {
            m_local_to_world_time = time;
            m_local_to_world = local_to_parent_at(time)
                * m_parent->local_to_world_at(time);
        }
        return m_local_to_world;
    }
    else
    {
        return local_to_parent_at(time);
    }
}

matrix4x4f const& transformation::local_to_parent_at(float time) const
{
    if (time != m_local_to_parent_time)
    {
        m_local_to_parent_time = time;
        m_local_to_parent = get_local_to_parent_at(time);
    }
    return m_local_to_parent;
}

bool transformation::is_constant() const
{
    return get_is_constant();
}

void transformation::invalidate()
{
    m_local_to_parent_time = -666.0f;
    m_local_to_world_time = -666.0f;
}

matrix4x4f local_to_parent(transformation const& child,
    transformation const* parent, float time)
{
    if (child.parent() && child.parent().get() != parent)
    {
        return child.local_to_parent_at(time)
            * local_to_parent(*child.parent(), parent, time);
    }
    else
    {
        return child.local_to_parent_at(time);
    }
}

}}} // namespace pt::d3d9::rendering
