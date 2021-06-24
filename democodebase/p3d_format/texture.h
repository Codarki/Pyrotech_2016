#ifndef PT_P3D_FORMAT_TEXTURE_H
#define PT_P3D_FORMAT_TEXTURE_H

#include "pt/basic_types.h"
#include "pt/vector.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/string.h"

namespace pt {
class parameter_node;
}

namespace p3d_format {

using namespace pt;

class chunk_reader;

class texture
{
public:
    texture();

    void read_contents(chunk_reader& chunk);

    std::shared_ptr<parameter_node> parameters() const;

private: // data members
    uint32 m_type_id;
    std::string m_name;
    float m_blur_percent;
    float m_unknown;
    int m_mapping_parameters;
    vector2f m_uv_scale;
    vector2f m_uv_offset;
};

} // p3d_format

#endif
