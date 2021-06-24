#ifndef PT_CONVERSION_FILE_P3D_WRITE_MESH_H
#define PT_CONVERSION_FILE_P3D_WRITE_MESH_H

#include "pt/path_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/map.h"
#include "pt/std/string.h"

namespace pt {
    class file_output_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {
    class object;
}}} // namespace pt::conversion::file_3ds

namespace pt { namespace conversion { namespace p3d {

std::string get_mesh_filename(boost::shared_ptr<file_3ds::object> const& obj,
    config_node const& config);

void write_scene_mesh(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s, path const& object_directory,
    std::map<std::string,std::string> const& mesh_names);

void write_mesh_model(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s);

}}} // namespace pt::conversion::p3d

#endif
