#include "texture.h"
#include "chunk_reader.h"
#include "id.h"
#include "pt/vector_utility.h"

#include "pt/animatable_value_utility.h"
#include "pt/parameter_node.h"
#include "pt/parameter_collection.h"
#include "pt/string_wrapper.h"

namespace p3d_format {

texture::texture()
{
}

void texture::read_contents(chunk_reader& parent)
{
    chunk_reader chunk(parent);

    m_type_id = chunk.id() ;//chunk.read<uint32>();
    //assert(m_type_id == id_texture_map_1);

    m_name = chunk.read<string_wrapper>().string();
    m_blur_percent = chunk.read<float>();
    m_unknown = chunk.read<float>();
    m_mapping_parameters = chunk.read<uint16>();
    m_uv_scale = chunk.read<vector2f>();
    m_uv_offset = chunk.read<vector2f>();
}

namespace {
    template<typename T>
    std::shared_ptr<parameter_node> create(std::string const& name, T const& value)
    {
        return std::shared_ptr<parameter_node>(new parameter_node(
            name, create_constant_value(value)));
    }
} // anonymous namespace

std::shared_ptr<parameter_node> texture::parameters() const
{
    std::shared_ptr<parameter_node> result = create("p3d texture", m_name);

    result->children().add(create("type_id", m_type_id));
    result->children().add(create("blur_percent", m_blur_percent));
    result->children().add(create("mapping_parameters", m_mapping_parameters));
    result->children().add(create("uv_scale", m_uv_scale));
    result->children().add(create("uv_offset", m_uv_offset));

    return result;
}

} // p3d_format
