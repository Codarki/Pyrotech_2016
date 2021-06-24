#include "object.h"
#include "common.h"
#include "mesh.h"
#include "light.h"
#include "camera.h"
#include "pt/bounded_input_stream.h"
#include "pt/log.h"
#include "pt/string_wrapper.h"
#include "pt/boost/format.h"

namespace pt { namespace conversion { namespace file_3ds {

object::object(std::string const& name)
:   m_name(name)
{
}

std::string const& object::name() const
{
    return m_name;
}

boost::shared_ptr<object> read_object(chunk_info const& parent_info,
    bounded_input_stream& s)
{
	boost::shared_ptr<object> obj;

    std::string name = read_c_str(s);

    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_TRIMESH))
        {
            log((boost::format("mesh %1% (%2$x)") % name % parent_info.id).str());
            logging::scoped_indent indent;

            obj = read_mesh(info, name, s);
        }
        else if (info.is(CHUNK_LIGHT))
        {
            log((boost::format("light %1% (%2$x)") % name % parent_info.id).str());
            logging::scoped_indent indent;

            obj = read_light(info, name, s);
        }
        else if (info.is(CHUNK_CAMERA))
        {
            log((boost::format("camera %1% (%2$x)") % name % parent_info.id).str());
            logging::scoped_indent indent;
            obj = read_camera(info, name, s);
        }
        else if (info.is(CHUNK_HIERARCHY))
        {
            log((boost::format("hierarchy %1% (%2$x)") % name % parent_info.id).str());
            logging::scoped_indent indent;
            //read_object(info, s);
        }
        else
        {
            log((boost::format("unknown %1% (%2$x)") % name % parent_info.id).str());
        }

        skip_chunk(info, s);
    }
    //assert(obj);
    return obj;
}

}}} // namespace pt::conversion::file_3ds
