#ifndef PT_P3D_FORMAT_SCENE_CACHE_H
#define PT_P3D_FORMAT_SCENE_CACHE_H

#include "pt/std/map.h"
#include "pt/boost/path.h"
#include "pt/boost/shared_ptr.h"

namespace pt {
class progress_report;
}

namespace p3d_format {

using namespace pt;
class scene;

class scene_cache
{
public:
    scene_cache(pt::progress_report* progress);

    shared_ptr<scene> find_or_load(path const& path);

    pt::progress_report* progress_report();

private: // type definitions
    typedef std::map<std::string,shared_ptr<scene> > map;
private: // data members
    map m_map;
    pt::progress_report* m_progress;
};

} // namespace p3d_format

#endif
