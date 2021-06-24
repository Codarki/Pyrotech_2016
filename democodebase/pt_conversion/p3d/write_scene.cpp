#include "write_scene.h"
#include "write_camera.h"
#include "write_light.h"
#include "write_material.h"
#include "write_mesh.h"
#include "pt/file_output_stream.h"
#include "../file_3ds/data.h"
#include "../file_3ds/object.h"
#include "p3d_format/id.h"
#include "p3d_format/version.h"
#include "pt/std/map.h"

namespace pt { namespace conversion { namespace p3d {

void write_keyframer(file_3ds::keyframer const& keyframer,
    file_output_stream& s);

mesh_to_filename_map get_mesh_filenames(file_3ds::data const& data,
    config_node const& config)
{
    mesh_to_filename_map result;

    for (file_3ds::data::object_container::const_iterator i =
        data.objects().begin(); i != data.objects().end(); ++i)
    {
        if (!(*i)->is_mesh())
            continue;

        std::string filename = get_mesh_filename(*i, config);

        result[(*i)->name()] = filename;
    }

    return result;
}

void write_scene_object(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s, path const& object_directory,
    mesh_to_filename_map const& mesh_names)
{
    size_t start_pos = s.current_pos();
    size_t chunk_len_offset = start_pos + sizeof(uint32);

    assert(obj);
    if (obj->is_camera())
    {
        write_scene_camera(obj, s);
    }
    if (obj->is_mesh())
    {
        write_scene_mesh(obj, s, object_directory, mesh_names);
    }
    if (obj->is_light())
    {
        write_scene_light(obj, s);
    }

    // write back the size of this chunk
    size_t current_pos = s.current_pos();
    size_t chunk_size = current_pos - start_pos;
    s.seek(chunk_len_offset);
    write(s, static_cast<uint32>(chunk_size));
    s.seek(current_pos);
}

void write_scene(path const& scene_filename,
	boost::shared_ptr<file_3ds::data> const& data, path const& object_directory,
    path const& image_directory, mesh_to_filename_map const& mesh_names)
{
    file_output_stream dst_stream(scene_filename);

    size_t start_pos = dst_stream.current_pos();
    size_t chunk_len_offset = start_pos + sizeof(uint32);

    // header
    write(dst_stream, static_cast<uint32>(p3d_format::id_scene));
    write(dst_stream, static_cast<uint32>(0)); // file_len unkown at this point.
    write(dst_stream, static_cast<uint16>(p3d_format::version_scene));
    for (size_t i = 0; i < 22; ++i)
    {
        write(dst_stream, static_cast<uint8>(0));
    }

    // chunks
    for (file_3ds::data::object_container::const_iterator i =
        data->objects().begin(); i != data->objects().end(); ++i)
    {
        write_scene_object(*i, dst_stream, object_directory, mesh_names);
    }

    // materials
    for (file_3ds::data::material_container::const_iterator i =
        data->materials().begin(); i != data->materials().end(); ++i)
    {
        write_scene_material(*i, dst_stream, image_directory);
    }

    write_keyframer(*data->keyframer(), dst_stream);

    // write back the size of this chunk
    size_t current_pos = dst_stream.current_pos();
    size_t chunk_size = current_pos - start_pos;
    dst_stream.seek(chunk_len_offset);
    write(dst_stream, static_cast<uint32>(chunk_size));
    dst_stream.seek(current_pos);
}

}}} // namespace pt::conversion::p3d
