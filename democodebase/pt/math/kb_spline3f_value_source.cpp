#include "kb_spline3f_value_source.h"

namespace pt { namespace math {

// kb_spline3f_value_source

kb_spline3f_value_source::kb_spline3f_value_source(
    kb_spline3f_data_set const& data)
:   m_spline(data)
{
}

bool kb_spline3f_value_source::is_constant() const
{
    // TODO
    return false;
}

vector3f kb_spline3f_value_source::value_at(float time) const
{
    return m_spline.value_at(time);
}

// catmull_rom_spline3f_value_source

catmull_rom_spline3f_value_source::catmull_rom_spline3f_value_source(
    catmull_rom_spline3f_data_set const& data)
:   m_spline(data)
{
}

bool catmull_rom_spline3f_value_source::is_constant() const
{
    // TODO
    return false;
}

vector3f catmull_rom_spline3f_value_source::value_at(float time) const
{
    return m_spline.value_at(time);
}

// catmull_rom_splinef_value_source

catmull_rom_splinef_value_source::catmull_rom_splinef_value_source(
    catmull_rom_data_set<float> data)
:   m_spline(data)
{
}

bool catmull_rom_splinef_value_source::is_constant() const
{
    return false;
}

float catmull_rom_splinef_value_source::value_at(float time) const
{
    return m_spline.value_at(time);
}

// kb_splinef_value_source

kb_splinef_value_source::kb_splinef_value_source(
    kb_splinef_data_set const& data)
:   m_spline(data)
{
}

bool kb_splinef_value_source::is_constant() const
{
    return false;
}

float kb_splinef_value_source::value_at(float time) const
{
    return m_spline.value_at(time);
}

}} // namespace pt::math
