#pragma once
#ifndef PT_D3D9_SHADERS_SURFACE_SHADER_COLLECTION_H
#define PT_D3D9_SHADERS_SURFACE_SHADER_COLLECTION_H

#include "surface_shader_fwd.h"
#include "pt/path.h"
#include "pt/std/map.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {

class surface_shader;
using shared_surface_shader = std::shared_ptr<surface_shader>;

class surface_shader_collection
{
public:
    std::shared_ptr<surface_shader> find(pt::path const& path);
    void add(pt::path const& path, std::shared_ptr<surface_shader> shader);

private: // type definitions
    typedef std::map<pt::path,std::shared_ptr<surface_shader> > map;

private: // data members
    map m_shaders;
};

}}} // namespace pt::d3d9::shaders

#endif
