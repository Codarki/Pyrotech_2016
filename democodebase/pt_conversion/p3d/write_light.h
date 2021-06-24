#ifndef PT_CONVERSION_FILE_P3D_WRITE_LIGHT_H
#define PT_CONVERSION_FILE_P3D_WRITE_LIGHT_H

#include "pt/boost/smart_ptr.h"

namespace pt {
    class file_output_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {
    class object;
}}} // namespace pt::conversion::file_3ds

namespace pt { namespace conversion { namespace p3d {

void write_scene_light(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s);

}}} // namespace pt::conversion::p3d

#endif
