#include "surface_shader.h"
#include "simple_surface_shader.h"

namespace pt { namespace d3d9 { namespace shaders {

surface_shader::~surface_shader()
{
}

std::shared_ptr<surface_shader> load_surface_shader(pt::path const& path)
{
    auto result = std::make_shared<shaders::simple_surface_shader>(path);
    return result;
}

}}} // namespace pt::d3d9::shaders
