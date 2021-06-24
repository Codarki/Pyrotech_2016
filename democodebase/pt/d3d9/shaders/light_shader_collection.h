#pragma once
#ifndef PT_D3D9_SHADERS_LIGHT_SHADER_COLLECTION_H
#define PT_D3D9_SHADERS_LIGHT_SHADER_COLLECTION_H

#include "light_shader_fwd.h"
#include "pt/path.h"
#include "pt/std/map.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {

class light_shader_collection
{
public:
    std::shared_ptr<light_shader> find(pt::path const& path);
    void add(pt::path const& path, std::shared_ptr<light_shader> shader);

private: // type definitions
    typedef std::map<pt::path,std::shared_ptr<light_shader>> map;

private: // data members
    map m_shaders;
};

}}} // namespace pt::d3d9::shaders

#endif
