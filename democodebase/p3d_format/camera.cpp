#include "camera.h"
#include "chunk_reader.h"
#include "pt/vector_utility.h"
#include "pt/string_wrapper.h"

namespace p3d_format {

camera::camera(chunk_reader& chunk)
{
    set_name(chunk.read<pt::string_wrapper>());

    m_position = chunk.read<vector3f>();
    m_target = chunk.read<vector3f>();
    m_bank = chunk.read<float>();
    m_lens = chunk.read<float>();
}

camera::camera(vector3f const &pos, vector3f const &target, float bank, float lens)
{
	m_position = pos;
	m_target = target;
	m_bank = bank;
	m_lens = lens;
}

vector3f const& camera::position() const
{
    return m_position;
}

vector3f const& camera::target() const
{
    return m_target;
}

float camera::bank() const
{
    return m_bank;
}

float camera::lens() const
{
    return m_lens;
}

} // namespace p3d_format
