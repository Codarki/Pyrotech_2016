#ifndef PT_MATH_STEP_CURVE_H
#define PT_MATH_STEP_CURVE_H

#include "data_set.h"
#include "pt/vector.h"
#include "pt/animatable_value_source.h"
#include "pt/string_wrapper.h"

namespace pt { namespace math {

template<typename ResultType>
class step_curve_base
{
public:
    virtual ~step_curve_base() {}

public:
    virtual ResultType value_at(float time) const = 0;
};

template<typename DataSet>
class step_curve : public step_curve_base<typename DataSet::result_type>
{
public:
    typedef typename DataSet::result_type result_type;

public:
    step_curve(DataSet const& data);
    DataSet const& dataset() const;

    result_type value_at(float time) const;

private: // data members
    DataSet m_data;
};

/// Step curve of text values.
typedef step_curve<data_set<std::string> > step_curve_string;


/// Text value which is animated by "step" interpolation. Time is always
/// clamped downwards to previous sample.
/// Also called "Piecewise constant interpolation"
/// Also called "Nearest-neighbor interpolation"
/// Also called "proximal interpolation"
/// Also called "point sampling"
class step_curve_string_value_source :
    public animatable_value_source<std::string>
{
public:
    step_curve_string_value_source(data_set<std::string> const& data);
    step_curve_string const& step_curve() const;

public: // interface required by animatable_value_source<std::string>
    virtual bool is_constant() const;
    virtual std::string value_at(float time) const;
    virtual string_wrapper type_name() const;

private: // data members
    step_curve_string m_curve;
};


// .inl

template<typename DataSet>
inline step_curve<DataSet>::step_curve(DataSet const& data)
: m_data(data)
{
}

template<typename DataSet>
DataSet const& step_curve<DataSet>::dataset() const
{
    return m_data;
}

template<typename DataSet>
inline typename step_curve<DataSet>::result_type
    step_curve<DataSet>::value_at(float time) const
{
    DataSet::const_iterator i = get_optional_endpoint(time, m_data);

    if (i == m_data.end())
    {
        DataSet::point_pair points = get_interval(time, m_data);

        if (points.second == m_data.end())
        {
            // exact match.
            return points.first->value();
        }
        return points.first->value();
    }
    else
    {
        return i->value();
    }
}

// step_curve_string_value_source

inline step_curve_string_value_source::step_curve_string_value_source(
    data_set<std::string> const& data)
: m_curve(data)
{
}

inline step_curve_string const& step_curve_string_value_source::step_curve() const
{
    return m_curve;
}

inline bool step_curve_string_value_source::is_constant() const
{
    return false;
}

inline std::string step_curve_string_value_source::value_at(float time) const
{
    return m_curve.value_at(time);
}

inline pt::string_wrapper step_curve_string_value_source::type_name() const
{
    return pt::string_wrapper("step_curve_string_value_source");
}

}} // namespace pt::math

#endif
