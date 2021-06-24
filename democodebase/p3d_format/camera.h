#ifndef PT_P3D_FORMAT_CAMERA_H
#define PT_P3D_FORMAT_CAMERA_H

#include "item.h"
#include "pt/vector.h"

namespace p3d_format {

class chunk_reader;

class camera : public item
{
public:
    camera(chunk_reader& chunk);
	camera(vector3f const & pos, vector3f const &target, float bank, float lens);

    vector3f const& position() const;
    vector3f const& target() const;
    float bank() const;
    float lens() const;

private:
    vector3f m_position;
    vector3f m_target;
    float m_bank;
    float m_lens;
};

} // namespace p3d_format

#endif
