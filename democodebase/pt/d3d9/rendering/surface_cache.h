#ifndef PT_D3D9_RENDERING_SURFACE_CACHE_H
#define PT_D3D9_RENDERING_SURFACE_CACHE_H

#include "pt/std/map.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace source {
    class material;
}
class surface;

class surface_cache
{
public:
	typedef std::shared_ptr<source::material const> key_type;

public:
    surface_cache();
    std::shared_ptr<surface> find_or_create(key_type key);

    std::string parameters_as_readable_text() const;

private: // type definitions
    typedef std::map<key_type,std::shared_ptr<surface> > surface_map;

private: // data members
    surface_map m_surfaces;
};

}}} // namespace pt::d3d9::rendering

#endif
