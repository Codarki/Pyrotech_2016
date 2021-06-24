#ifndef PT_CONVERSION_FILE_P3D_VERSION_H
#define PT_CONVERSION_FILE_P3D_VERSION_H

namespace pt { namespace conversion { namespace p3d {

#define PT_CREATE_VERSION(major, minor) ((major << 8) + minor)
#define PT_VERSION_MAJOR(version) (version >> 8)
#define PT_VERSION_MINOR(version) (version & 0xff)

enum {
    version_scene = PT_CREATE_VERSION(0, 01),
    version_model = PT_CREATE_VERSION(0, 01),
};

#undef PT_CREATE_VERSION

}}} // namespace pt::conversion::p3d

#endif
