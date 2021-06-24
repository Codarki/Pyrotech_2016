#ifndef PT_CONVERSION_FILE_P3D_WRITE_MATERIAL_H
#define PT_CONVERSION_FILE_P3D_WRITE_MATERIAL_H

#include "pt/path_fwd.h"
#include "pt/boost/shared_ptr.h"

namespace pt {
    class file_output_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {
    class material;
}}} // namespace pt::conversion::file_3ds

namespace pt { namespace conversion { namespace p3d {

void write_scene_material(boost::shared_ptr<file_3ds::material> const& mat,
    file_output_stream& s, path const& image_directory);

}}} // namespace pt::conversion::p3d

#endif
