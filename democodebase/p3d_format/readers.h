#ifndef PT_P3D_FORMAT_READERS_H
#define PT_P3D_FORMAT_READERS_H

#include "pt/path_fwd.h"
#include "pt/boost/shared_ptr.h"

namespace pt {
    class progress_report;
}

namespace p3d_format {

class mesh;
class scene;

boost::shared_ptr<scene> read_scene(pt::path const& path,
    pt::progress_report* progress, bool only_serialize);
boost::shared_ptr<mesh> read_mesh(pt::path const& path,
    pt::progress_report* progress);

} // namespace p3d_format

#endif
