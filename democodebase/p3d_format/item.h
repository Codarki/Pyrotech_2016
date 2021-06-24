#ifndef PT_P3D_FORMAT_ITEM_H
#define PT_P3D_FORMAT_ITEM_H

#include "pt/string_wrapper.h"
#include "pt/boost/shared_ptr.h"

namespace p3d_format {

using namespace pt;

class animation;

class item
{
public:
    void set_name(string_wrapper const& name);
    std::string const& name() const;

	void set_animation(boost::shared_ptr<p3d_format::animation> animation);
	boost::shared_ptr<p3d_format::animation> animation() const;
	boost::shared_ptr<p3d_format::animation> animation();

	void set_target_animation(boost::shared_ptr<p3d_format::animation> animation);
	boost::shared_ptr<p3d_format::animation> target_animation() const;
    boost::shared_ptr<p3d_format::animation> target_animation();

private:
    std::string m_name;
	boost::shared_ptr<p3d_format::animation> m_animation;
	boost::shared_ptr<p3d_format::animation> m_target_animation;
};

} // namespace p3d_format

#endif
