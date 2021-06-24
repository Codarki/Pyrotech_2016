#include "write_light.h"
#include "pt/file_output_stream.h"
#include "pt/vector_utility.h"
#include "../file_3ds/light.h"
#include "p3d_format/chunk_writer.h"

namespace pt { namespace conversion { namespace p3d {

namespace {
    vector3f rh_to_lh(vector3f const& v)
    {
        // Flip y and z elements.
        return vector3f(v[0], v[2], v[1]);
        //return vector3f(v[0], v[1], v[2]);
    }
} // anonymous namespace

void write_scene_light(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s)
{
	boost::shared_ptr<file_3ds::light> light =
        boost::dynamic_pointer_cast<file_3ds::light>(obj);

    assert(light);

    p3d_format::chunk_writer chunk(s, p3d_format::id_light);

    chunk.write(light->name());
    chunk.write<vector3f>(rh_to_lh(light->position()));
    chunk.write<vector3f>(light->color());
    chunk.write<vector2f>(light->range());
    chunk.write<float>(light->multiplier());

    chunk.write<bool>(light->is_spotlight());
    if (light->is_spotlight())
    {
        chunk.write<vector3f>(rh_to_lh(light->target()));
        chunk.write<float>(light->hotspot());
        chunk.write<float>(light->falloff());
    }
}

}}} // namespace pt::conversion::p3d
