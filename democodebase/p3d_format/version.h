#ifndef PT_P3D_FORMAT_VERSION_H
#define PT_P3D_FORMAT_VERSION_H

namespace p3d_format {

#define PT_CREATE_VERSION(major, minor) ((major << 8) + minor)
#define PT_VERSION_MAJOR(version) (version >> 8)
#define PT_VERSION_MINOR(version) (version & 0xff)

enum {
    version_scene = PT_CREATE_VERSION(0, 03),
    version_model = PT_CREATE_VERSION(0, 03),
};

#undef PT_CREATE_VERSION

} // namespace p3d_format

#endif
