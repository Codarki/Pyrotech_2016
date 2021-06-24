#include "write_camera.h"
#include "pt/file_output_stream.h"
#include "pt/vector_utility.h"
#include "../file_3ds/camera.h"
#include "p3d_format/chunk_writer.h"

namespace pt { namespace conversion { namespace p3d {

namespace {
    vector3f rh_to_lh(vector3f const& v)
    {
        // Flip y and z elements.
        return vector3f(v[0], v[2], v[1]);
       //return vector3f(v[0],v[1],v[2]);
    }
} // anonymous namespace

void write_scene_camera(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s)
{
	boost::shared_ptr<file_3ds::camera> camera =
        boost::dynamic_pointer_cast<file_3ds::camera>(obj);

    assert(camera);

    p3d_format::chunk_writer chunk(s, p3d_format::id_camera);

    chunk.write(camera->name());
    chunk.write<vector3f>(rh_to_lh(camera->position()));
    chunk.write<vector3f>(rh_to_lh(camera->target()));
    chunk.write<float>(camera->bank());
    chunk.write<float>(camera->lens());
}

}}} // namespace pt::conversion::p3d
