#ifndef PT_D3D9_RENDERING_LIGHT_PROPERTIES_CACHE_H
#define PT_D3D9_RENDERING_LIGHT_PROPERTIES_CACHE_H

#include "pt/std/map.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace source {
    class light_material;
}

class light_properties;
//class light_shader_parameters;

class light_properties_cache
{
public:
    typedef std::shared_ptr<source::light_material const> key_type;
    //typedef std::shared_ptr<light_shader_parameters const> key_type;

public:
    light_properties_cache();
    std::shared_ptr<light_properties> find_or_create(key_type key);

    std::string parameters_as_readable_text() const;

private: // type definitions
    typedef std::map<key_type,std::shared_ptr<light_properties>> light_properties_map;

private: // data members
    light_properties_map m_light_properties;
};

}}} // namespace pt::d3d9::rendering

#endif
