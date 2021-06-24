#include "spline_parameter.h"
#include "pt/config_node.h"
#include "pt/math/cubic_hermite_spline.h"

namespace content {

void spline_parameter<float>::set_default_value(float value)
{
    m_default_value = value;
}

void spline_parameter<float>::set_spline(std::shared_ptr<content::splinef> spline)
{
    m_spline = spline;
}

float spline_parameter<float>::value_at(float time) const
{
    if (m_spline)
    {
        return m_spline->value_at(time);
    }
    else
    {
        return m_default_value;
    }
}

//spline_parameter<float> make_spline_parameter(config_node const& config,
//    curve_manager& curves)
//{
//    spline_parameter<float> result;
//
//    config_node::const_iterator i = config.find_child("curve");
//    if (i != config.end())
//    {
//        result.set_spline(curves.splinef(i->value()));
//    }
//    else
//    {
//        float value;
//        config.parse(value);
//        result.set_default_value(value);
//    }
//
//    return result;
//}

} // namespace content
