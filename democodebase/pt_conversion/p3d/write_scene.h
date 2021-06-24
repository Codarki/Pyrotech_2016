#ifndef PT_CONVERSION_FILE_P3D_WRITE_SCENE_H
#define PT_CONVERSION_FILE_P3D_WRITE_SCENE_H

#include "pt/path_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/map.h"
#include "pt/std/string.h"

namespace pt {
    class file_output_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {
    class data;
    class object;
    class material;
}}} // namespace pt::conversion::file_3ds

namespace pt { namespace conversion { namespace p3d {

typedef std::map<std::string,std::string> mesh_to_filename_map;

mesh_to_filename_map get_mesh_filenames(file_3ds::data const& data,
    config_node const& config);

void write_scene_object(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s, path const& object_directory,
    config_node const& config);

void write_scene_material(boost::shared_ptr<file_3ds::material> const& obj,
    file_output_stream& s, path const& image_directory);

void write_scene(path const& scene_filename,
	boost::shared_ptr<file_3ds::data> const& data, path const& object_directory,
    path const& image_directory, mesh_to_filename_map const& mesh_names);

}}} // namespace pt::conversion::p3d

#endif
