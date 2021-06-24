#ifndef PT_P3D_FORMAT_MATERIAL_H
#define PT_P3D_FORMAT_MATERIAL_H

#include "texture.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/vector.h"

namespace pt {
class parameter_node;
}

namespace p3d_format {

using namespace pt;

class chunk_reader;

class material
{
public:
    material(chunk_reader& chunk);

    std::shared_ptr<parameter_node> parameters() const;

private: // data members
    std::string m_name;
    vector3f m_ambient;
    vector3f m_diffuse;
    vector3f m_specular;

    float m_shininess;
    float m_shininess_strength;
    float m_transparency;
    float m_transparency_falloff;
    float m_reflection_blur;

    std::vector<texture> m_textures;
};

} // p3d_format

#endif
