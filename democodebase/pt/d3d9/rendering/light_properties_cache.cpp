#include "light_properties_cache.h"
#include "light_properties.h"
#include "light_variables2.h"
#include "source/light_material.h"
#include "pt/parameter_node.h"
#include "pt/animatable_value_base.h"
#include "pt/string_wrapper.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    std::shared_ptr<light_properties> build_shared_light_properties(
        light_shader_parameters const& parameters)
    {
        auto variables = std::make_shared<light_variables2>(parameters);
        return std::make_shared<light_properties>(variables);
    }
} // anonymous namespace

light_properties_cache::light_properties_cache()
{
}

std::shared_ptr<light_properties> light_properties_cache::find_or_create(
    key_type key)
{
    auto i = m_light_properties.find(key);
    if (i != m_light_properties.end())
        return i->second;

    auto result = build_shared_light_properties(key->shader_parameters());
    m_light_properties.insert(light_properties_map::value_type(key, result));
    return result;
}

std::string light_properties_cache::parameters_as_readable_text() const
{
    std::string result;
    result.reserve(200);

    for (auto const& i : m_light_properties)
    {
        //light_shader_parameters const& parameters = *i.first;
        light_properties const& light_properties = *i.second;

        //result += material.name() + "\r\n";
        result += hierarchy_as_readable_text(light_properties.parameters(), 2).string() + "\r\n";
    }

    return result;
}

}}} // namespace pt::d3d9::rendering
