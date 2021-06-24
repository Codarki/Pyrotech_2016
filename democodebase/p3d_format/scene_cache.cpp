#include "scene_cache.h"
#include "readers.h"

namespace p3d_format {

scene_cache::scene_cache(pt::progress_report* progress)
: m_progress(progress)
{
}

shared_ptr<p3d_format::scene> scene_cache::find_or_load(path const& path)
{
    map::iterator i = m_map.find(path.string());
    if(i != m_map.end())
        return i->second;

    shared_ptr<p3d_format::scene> scene
        = p3d_format::read_scene(path, m_progress);

    std::pair<map::iterator, bool> result
        = m_map.insert(map::value_type(path.string(), scene));
    assert(result.second);

    return scene;
}

pt::progress_report* scene_cache::progress_report()
{
    return m_progress;
}

} // namespace p3d_format
