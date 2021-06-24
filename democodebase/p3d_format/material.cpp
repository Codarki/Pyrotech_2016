#include "material.h"
#include "chunk_reader.h"
#include "id.h"
#include "pt/basic_types.h"
#include "pt/input_stream.h"
#include "pt/vector_utility.h"

#include "pt/animatable_value_utility.h"
#include "pt/parameter_node.h"
#include "pt/parameter_collection.h"
#include "pt/string_wrapper.h"

namespace p3d_format {

material::material(chunk_reader& chunk)
{
    assert(chunk.id() == id_material);
    assert(!chunk.end());

    m_name = chunk.read<pt::string_wrapper>().string();
    m_ambient = chunk.read<vector3f>();
    m_diffuse = chunk.read<vector3f>();
    m_specular = chunk.read<vector3f>();
    m_shininess = chunk.read<float>();
    m_shininess_strength = chunk.read<float>();
    m_transparency = chunk.read<float>();
    m_transparency_falloff = chunk.read<float>();
    m_reflection_blur = chunk.read<float>();
    int map_count = chunk.read<int8>();

    m_textures.resize(map_count);

    for (size_t i = 0; i < static_cast<size_t>(map_count); ++i)
    {
        m_textures[i].read_contents(chunk);
    }
}

namespace {
    template<typename T>
    std::shared_ptr<parameter_node> create(std::string const& name, T const& value)
    {
        return std::shared_ptr<parameter_node>(
            new parameter_node(name, create_constant_value(value)));
    }
} // anonymous namespace

std::shared_ptr<parameter_node> material::parameters() const
{
    std::shared_ptr<parameter_node> result = create("p3d material", m_name);

    result->children().add(create("ambient", m_ambient));
    result->children().add(create("diffuse", m_diffuse));
    result->children().add(create("specular", m_specular));
    result->children().add(create("shininess", m_shininess));
    result->children().add(create("shininess_strength", m_shininess_strength));
    result->children().add(create("transparency", m_transparency));
    result->children().add(create("transparency_falloff", m_transparency_falloff));
    result->children().add(create("reflection_blur", m_reflection_blur));

    if (!m_textures.empty())
    {
        std::shared_ptr<parameter_node> maps = create("maps", 0);
        for (std::vector<texture>::const_iterator i = m_textures.begin();
            i != m_textures.end(); ++i)
        {
            maps->children().add(i->parameters());
        }

        result->children().add(maps);
    }

    return result;
}

} // namespace p3d_format
