#include "light_shader_collection.h"

namespace pt { namespace d3d9 { namespace shaders {

std::shared_ptr<light_shader> light_shader_collection::find(pt::path const& path)
{
    auto i = m_shaders.find(path);
    if (i != m_shaders.end())
        return i->second;
    return nullptr;
}

void light_shader_collection::add(pt::path const& path, std::shared_ptr<light_shader> shader)
{
    m_shaders.insert(map::value_type(path, shader));
}

}}} // namespace pt::d3d9::shaders
