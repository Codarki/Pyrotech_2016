#ifndef PT_CONVERSION_FILE_P3D_WRITE_H
#define PT_CONVERSION_FILE_P3D_WRITE_H

#include "pt/path.h"
#include "pt/config_node_fwd.h"
#include "pt/boost/smart_ptr.h"

namespace pt {
    class bounded_input_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {
    class data;
}}} // namespace pt::conversion::file_3ds

namespace pt { namespace conversion { namespace p3d {

/// Writes a new .p3d_scene \a scene_filename.
void write(pt::path const& scene_filename, pt::path const& object_directory,
	pt::path const& image_directory, boost::shared_ptr<file_3ds::data> const& data,
    config_node const& config);

}}} // namespace pt::conversion::p3d

#endif
