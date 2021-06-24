#ifndef PT_MATH_KB_SPLINE3F_VALUE_SOURCE_H
#define PT_MATH_KB_SPLINE3F_VALUE_SOURCE_H

#include "cubic_hermite_spline.h"
#include "../animatable_value_source.h"

namespace pt { namespace math {

/// Source for animatable value.
class kb_spline3f_value_source : public animatable_value_source<vector3f>
{
public:
    kb_spline3f_value_source(kb_spline3f_data_set const& data);

public: // interface required by animatable_value_source<vector3f>
    bool is_constant() const;
    vector3f value_at(float time) const;

private: // data members
    kb_spline3f m_spline;
};

/// Source for animatable value.
class catmull_rom_spline3f_value_source :
    public animatable_value_source<vector3f>
{
public:
    catmull_rom_spline3f_value_source(
        catmull_rom_spline3f_data_set const& data);

public: // interface required by animatable_value_source<vector3f>
    bool is_constant() const;
    vector3f value_at(float time) const;

private: // data members
    catmull_rom_spline3f m_spline;
};

///
class catmull_rom_splinef_value_source :
    public animatable_value_source<float>
{
public:
    catmull_rom_splinef_value_source(
        catmull_rom_data_set<float> data);

public: // interface required by animatable_value_source<vector3f>
    bool is_constant() const;
    float value_at(float time) const;

private: // data members
    catmull_rom_splinef m_spline;
};

///
class kb_splinef_value_source :
    public animatable_value_source<float>
{
public:
    kb_splinef_value_source(kb_splinef_data_set const& data);

public: // interface required by animatable_value_source<vector3f>
    bool is_constant() const;
    float value_at(float time) const;

private: // data members
    kb_splinef m_spline;
};

}} // namespace pt::math

#endif
