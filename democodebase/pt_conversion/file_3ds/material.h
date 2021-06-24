#ifndef PT_CONVERSION_FILE_3DS_MATERIAL_H
#define PT_CONVERSION_FILE_3DS_MATERIAL_H

#include "pt/std/string.h"
#include "pt/std/map.h"
#include "pt/boost/smart_ptr.h"
#include "pt/vector.h"

namespace pt {
class bounded_input_stream;
    
namespace conversion { namespace file_3ds {

struct chunk_info;
class material_map;

/// 3ds material, consisting several maps.
class material
{
public: // type definitions
	typedef std::map<unsigned int, boost::shared_ptr<material_map> > map_map;

public:
    material();

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    std::string const& name() const;
    vector3f const& ambient() const;
    vector3f const& diffuse() const;
    vector3f const& specular() const;
    float shininess() const;
    float shininess_strength() const;
    float transparency() const;
    float transparency_falloff() const;
    float reflection_blur() const;

    map_map const& maps() const;
    
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

    map_map m_maps;
};

boost::shared_ptr<material> read_material(chunk_info const& parent_info,
    bounded_input_stream& s);

}}} // namespace pt::conversion::file_3ds

#endif
