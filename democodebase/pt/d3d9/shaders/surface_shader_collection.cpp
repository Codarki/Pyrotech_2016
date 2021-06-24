#include "surface_shader_collection.h"
#include "surface_shader.h"

namespace pt { namespace d3d9 { namespace shaders {

std::shared_ptr<surface_shader> surface_shader_collection::find(
    pt::path const& path)
{
    map::const_iterator i = m_shaders.find(path);
    if (i != m_shaders.end())
        return i->second;
    return nullptr;
}

void surface_shader_collection::add(pt::path const& path,
    std::shared_ptr<surface_shader> shader)
{
    m_shaders.insert(map::value_type(path, shader));
}

}}} // namespace pt::d3d9::shaders
