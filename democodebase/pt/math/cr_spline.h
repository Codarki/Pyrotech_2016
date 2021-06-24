#ifndef PT_MATH_CR_SPLINE_H
#define PT_MATH_CR_SPLINE_H

#include "cubic_hermite_spline.h"
#include "cr_data_set.h"
#include "spline_value_source.h"

namespace pt { namespace math {

template<typename T, int C>
class cr_spline : public cubic_hermite_spline<cr_data_set<T>,C>
{
public:
    cr_spline(cr_data_set<T> const data[C])
    :   cubic_hermite_spline<cr_data_set<T>,C>(data)
    {
    }
};

template<typename T>
class cr_spline<T,1> : public cubic_hermite_spline<cr_data_set<T>,1>
{
public:
    cr_spline(cr_data_set<T> const data)
    :   cubic_hermite_spline<cr_data_set<T>,1>(data)
    {
    }
};

typedef cr_spline<float,1> cr_splinef;
typedef cr_spline<float,2> cr_spline2f;
typedef cr_spline<float,3> cr_spline3f;
typedef cr_spline<float,4> cr_spline4f;

typedef spline_value_source<cr_spline<float,1> > cr_splinef_value_source;
typedef spline_value_source<cr_spline<float,2> > cr_spline2f_value_source;
typedef spline_value_source<cr_spline<float,3> > cr_spline3f_value_source;
typedef spline_value_source<cr_spline<float,4> > cr_spline4f_value_source;

}} // namespace pt::math

#endif
