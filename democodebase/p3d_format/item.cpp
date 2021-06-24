#include "item.h"

namespace p3d_format {

void item::set_name(string_wrapper const& name)
{
    m_name = name.string();
}

std::string const& item::name() const
{
    return m_name;
}

void item::set_animation(boost::shared_ptr<p3d_format::animation> animation)
{
	m_animation = animation;
}

boost::shared_ptr<p3d_format::animation> item::animation() const
{
    return m_animation;
}

boost::shared_ptr<p3d_format::animation> item::animation()
{
    return m_animation;
}

void item::set_target_animation(boost::shared_ptr<p3d_format::animation> animation)
{
	m_target_animation = animation;
}

boost::shared_ptr<p3d_format::animation> item::target_animation() const
{
    return m_target_animation;
}

boost::shared_ptr<p3d_format::animation> item::target_animation()
{
    return m_target_animation;
}

} // namespace p3d_format
