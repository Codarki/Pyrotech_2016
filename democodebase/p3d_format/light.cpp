#include "light.h"
#include "chunk_reader.h"
#include "pt/vector_utility.h"

namespace p3d_format {

light::light(chunk_reader& chunk)
{
    set_name(chunk.read<pt::string_wrapper>());

    m_position = chunk.read<vector3f>();
    m_color = chunk.read<vector3f>();
    m_range = chunk.read<vector2f>();
    m_multiplier = chunk.read<float>();

    m_is_spot = chunk.read<bool>();
    if (m_is_spot)
    {
        m_target = chunk.read<vector3f>();
        m_hotspot = chunk.read<float>();
        m_falloff = chunk.read<float>();
    }
    else
    {
        // These shouldn't be used anywhere for non-spots.
        m_target = vector3f();
        m_hotspot = 25.0f;
        m_falloff = 35.0f;
    }
}

vector3f const& light::position() const
{
    return m_position;
}

vector3f const& light::color() const
{
    return m_color;
}

vector3f const& light::target() const
{
    return m_target;
}

bool light::is_spot() const
{
    return m_is_spot;
}

float light::hotspot() const
{
    return m_hotspot;
}

float light::falloff() const
{
    return m_falloff;
}

vector2f const& light::range() const
{
    return m_range;
}

float light::multiplier() const
{
    return m_multiplier;
}

} // namespace p3d_format
