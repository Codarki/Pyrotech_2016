#if 0
#ifndef CONTENT_SEQUENCES_KIMMO_CYLINDER_DEFORM_H
#define CONTENT_SEQUENCES_KIMMO_CYLINDER_DEFORM_H

#include "leaf_sequence.h"
#include "pt/d3d9/device_child.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 {
    class graphics_device;
    class pixel_shader;
    class vertex_shader;

    namespace rendering {
        class item;
        class shading_program;
        class simple_transformation;
    }
}}

namespace content { namespace sequences {

class variables;

class kimmo_cylinder_deform : public simple_leaf_sequence,
    d3d9::device_reference
{
public:
    kimmo_cylinder_deform(d3d9::graphics_device& device,
        config_node const& config);

public: // interface required by sequence
    std::string const& name() const;
    shared_ptr<d3d9::texture_2d> draw(rendertarget& target, float time);

public: // interface required by composite_sequence
    void parse_inputs(config_node const& config);

private: // type definitions
    typedef std::vector<shared_ptr<d3d9::rendering::simple_transformation> >
        bone_transformations;

private: // data members
    std::string m_name;
    shared_ptr<d3d9::rendering::item> m_item;

    shared_ptr<d3d9::rendering::shading_program> m_cylinder_program;
    shared_ptr<d3d9::rendering::shading_program> m_bone_program;
    shared_ptr<variables> m_variables;

    float m_time;
    shared_ptr<d3d9::rendering::simple_transformation> m_transformation;
    bone_transformations m_bone_transformations;
};

}} // namespace content::sequences

#endif
#endif
