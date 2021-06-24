#ifndef PT_P3D_FORMAT_LIGHT_H
#define PT_P3D_FORMAT_LIGHT_H

#include "item.h"
#include "pt/vector.h"

namespace p3d_format {

class chunk_reader;

class light : public item
{
public:
    light(chunk_reader& chunk);

    vector3f const& position() const;
    vector3f const& color() const;
    vector3f const& target() const;
    bool is_spot() const;
    float hotspot() const;
    float falloff() const;
    vector2f const& range() const;
    float multiplier() const;

private:
    vector3f m_position;
    vector3f m_color;
    vector3f m_target;
    bool m_is_spot;
    float m_hotspot;
    float m_falloff;
    vector2f m_range;
    float m_multiplier;
};

} // namespace p3d_format

#endif
