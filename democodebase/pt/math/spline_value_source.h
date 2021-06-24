#ifndef PT_MATH_SPLINE_VALUE_SOURCE_H
#define PT_MATH_SPLINE_VALUE_SOURCE_H

#include "pt/animatable_value_source.h"
#include "pt/string_wrapper.h"

namespace pt { namespace math {

template<typename Spline>
class spline_value_source :
    public animatable_value_source<typename Spline::result_type>
{
public:
    typedef typename Spline::result_type result_type;

public:
    spline_value_source(Spline const& spline)
    :   m_spline(spline)
    {
    }

public: // interface required by animatable_value_source<vector3f>
    virtual bool is_constant() const
    {
        return false;
    }

    virtual result_type value_at(float time) const
    {
        return m_spline.value_at(time);
    }

    virtual string_wrapper type_name() const
    {
        return string_wrapper("spline_value_source");
    }

private: // data members
    Spline m_spline;
};

}} // namespace pt::math

#endif
