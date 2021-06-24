#ifndef CONTENT_SPLINE_PARAMETER_H
#define CONTENT_SPLINE_PARAMETER_H

#include "curve_free_functions.h"

namespace content {

template<typename T>
class spline_parameter;

template<>
class spline_parameter<float>
{
public:
    void set_default_value(float value);
	void set_spline(std::shared_ptr<content::splinef> spline);

    float value_at(float time) const;

private: // data members
    float m_default_value;
	std::shared_ptr<content::splinef> m_spline;
};

//spline_parameter<float> make_spline_parameter(config_node const& config,
//    curve_manager& curves);

} // namespace content

#endif
