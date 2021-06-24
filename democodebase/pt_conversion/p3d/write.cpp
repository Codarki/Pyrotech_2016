#include "write.h"
#include "write_mesh.h"
#include "write_scene.h"
#include "../file_3ds/data.h"
#include "../file_3ds/object.h"
#include "pt/file_output_stream.h"

namespace pt { namespace conversion { namespace p3d {

void write(path const& scene_filename, path const& object_directory,
	path const& image_directory, boost::shared_ptr<file_3ds::data> const& data,
    config_node const& config)
{
    std::string scene_name = scene_filename.stem().string();
    path obj_dir = object_directory / scene_name;
    pt::create_directories(obj_dir);

    mesh_to_filename_map mesh_names = get_mesh_filenames(*data, config);

    write_scene(scene_filename, data, obj_dir, image_directory, mesh_names);

    // Write each mesh into own file.

    std::vector<std::string> written_models;

    for (file_3ds::data::object_container::const_iterator i =
        data->objects().begin(); i != data->objects().end(); ++i)
    {
        if ((*i)->is_mesh())
        {
            mesh_to_filename_map::const_iterator j
                = mesh_names.find((*i)->name());
            assert(j != mesh_names.end());

            std::string mesh_filename = j->second;

            std::vector<std::string>::const_iterator k = std::find(
                written_models.begin(), written_models.end(), mesh_filename);
            if (k != written_models.end())
                continue;

            path mesh_path = obj_dir / mesh_filename; //((*i)->name() + ".p3d_model");
            file_output_stream dst_stream(mesh_path);

            write_mesh_model(*i, dst_stream);

            written_models.push_back(mesh_filename);
        }
    }
}

}}} // namespace pt::conversion::p3d
