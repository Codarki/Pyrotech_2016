#include "camera.h"
#include "common.h"
#include "pt/vector_utility.h"
#include "pt/bounded_input_stream.h"
#include "pt/log.h"

namespace pt { namespace conversion { namespace file_3ds {

camera::camera(std::string const& name)
:   object(name)
{
}

void camera::read(chunk_info const& , bounded_input_stream& s)
{
    m_position = pt::read<vector3f>(s);
    m_target = pt::read<vector3f>(s);
    m_bank = pt::read<float>(s);
    m_lens = pt::read<float>(s);
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

boost::shared_ptr<camera> read_camera(chunk_info const& parent_info,
    std::string const& name, bounded_input_stream& stream)
{
	boost::shared_ptr<camera> camera(new camera(name));
    camera->read(parent_info, stream);
    return camera;
}

}}} // namespace pt::conversion::file_3ds
