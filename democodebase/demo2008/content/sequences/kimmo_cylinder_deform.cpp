#if 0
#include "kimmo_cylinder_deform.h"
#include "rendertarget.h"
#include "detail/cylinder.h"
#include "detail/deformer.h"
#include "pt/config_node.h"

#include "pt/d3d9/error.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/utility.h"
#include "pt/exceptions/string_context.h"

#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/rendering/constant_setter_utility.h"
#include "pt/d3d9/rendering/drawable.h"
#include "pt/d3d9/rendering/geometry.h"
#include "pt/d3d9/rendering/item.h"
#include "pt/d3d9/rendering/transformer.h"
#include "pt/d3d9/rendering/simple_array_setter.h"
#include "pt/d3d9/rendering/simple_transformation.h"
#include "pt/d3d9/rendering/simple_variable_owner.h"
#include "pt/d3d9/rendering/shading_program.h"
#include "pt/d3d9/rendering/view.h"

#include "pt/d3d9/shaders/basic_surface_shader.h"
#include "pt/d3d9/shaders/stream_collection.h"

namespace content { namespace sequences {

shared_ptr<d3d9::rendering::item> build_deformer()
{
    // TODO create mesh_builder?
    d3d9::rendering::transformer_builder transformer_builder(
        make_shared_ptr(new d3d9::rendering::simple_transformation));

    shared_ptr<d3d9::rendering::item> result(new d3d9::rendering::item(
        "deformer_01", transformer_builder.shared_transformer()));

    return result;
}

class cylinder_surface_shader : public d3d9::shaders::basic_surface_shader
{
public:
    cylinder_surface_shader()
    :   d3d9::shaders::basic_surface_shader("cylinder_shader")
    {
        set_vertex_shader("main_vs",
            "struct input_vertex\n"
            "{\n"
            "    float4 position : POSITION;\n"
            "    int4 indices : BLENDINDICES;\n"
            "    float4 weight : BLENDWEIGHT;\n"
            "    float2 texcoord : TEXCOORD0;\n"
            "};\n"
            "struct output_vertex\n"
            "{\n"
            "    float4 position : POSITION;\n"
            "    float2 texcoord : TEXCOORD0;\n"
            "};\n"
            "float4x4 object_to_world;\n"
            "float4x4 world_to_view_clip;\n"
            "float4x4 bones[5];\n"

            "void main_vs(in input_vertex input, out output_vertex output)\n"
            "{\n"
            "    output.position = mul(input.position, bones[input.indices.x])\n"
            "        * input.weight.x;\n"
            "    output.position += mul(input.position, bones[input.indices.y])\n"
            "        * input.weight.y;\n"

            "    output.texcoord = input.weight.xy;\n"
            "    output.position = mul(output.position, object_to_world);\n"
            "    output.position = mul(output.position, world_to_view_clip);\n"
            "}\n");

        set_pixel_shader("main_ps",
            "void main_ps(\n"
            "    float2 texcoord : TEXCOORD0,\n"
            "    out float4 color : COLOR0)\n"
            "{\n"
            "    color.xy = texcoord.xy;\n"
            "    color.z = 0;\n"
            "    color.w = 1.0;\n"
            "}\n");
    }
};

class bone_surface_shader : public d3d9::shaders::basic_surface_shader
{
public:
    bone_surface_shader()
    :   d3d9::shaders::basic_surface_shader("bone_shader")
    {
        set_vertex_shader("main_vs",
            "struct input_vertex\n"
            "{\n"
            "    float4 position : POSITION;\n"
            "    float4 color : COLOR0;\n"
            "};\n"
            "float4x4 object_to_world;\n"
            "float4x4 world_to_view_clip;\n"

            "void main_vs(inout input_vertex input)\n"
            "{\n"
            "    input.position = mul(input.position, object_to_world);\n"
            "    input.position = mul(input.position, world_to_view_clip);\n"
            "}\n");

        set_pixel_shader("main_ps",
        "void main_ps(\n"
        "    float4 color : COLOR0,\n"
        "    out float4 out_color : COLOR0)\n"
        "{\n"
        "    out_color.xyz = color.xyz;\n"
        "    out_color.w = 1.0;\n"
        "}\n");
    }
};

class variables : public d3d9::rendering::simple_variable_owner
{
public:
    variables()
    :   m_bones(new d3d9::rendering::simple_array_setter<matrix4x4f,5>)
    {
    }
    void set_world_to_view_clip(matrix4x4f const& m)
    {
        m_world_to_view_clip = m;
    }

protected: // interface required by d3d9::rendering::simple_variable_owner
    shared_ptr<d3d9::rendering::constant_setter> on_create_constant(
        d3d9::constant const& constant) const
    {
        if (constant.name() == "bones")
        {
            //assert(!m_bones);
            return m_bones;
            //return d3d9::rendering::create_reference_setter<matrix4x4f>(
            //    m_world_to_view_clip, constant);
        }
        //if (constant.name() == "world_to_view_clip")
        //{
        //    return d3d9::rendering::create_reference_setter<matrix4x4f>(
        //        m_world_to_view_clip, constant);
        //}

        return shared_ptr<d3d9::rendering::constant_setter>();
    }
private: // data members
    matrix4x4f m_world_to_view_clip;
    //matrix4x4f m_bones[5];
    shared_ptr<d3d9::rendering::simple_array_setter<matrix4x4f,5> > m_bones;
};

namespace {
    matrix4x4f create_projection()
    {
        float fov_y_degrees = 45.0f;
        float fov_y = fov_y_degrees / 360.0f * (2 * 3.1415f);
        float aspect = 1280.0f / static_cast<float>(768.0f);
        float zn = 1.0f;
        float zf = 10000.0f;

        D3DXMATRIX mat;
        D3DXMatrixPerspectiveFovLH(&mat, fov_y, aspect, zn, zf);
        
        return d3d9::convert(mat);
    }

    shared_ptr<d3d9::rendering::item> create_cylinder_item(
        shared_ptr<d3d9::rendering::transformation> transformation)
    {
        using namespace d3d9::rendering;

        transformer_builder transformer_builder(transformation);

        shared_ptr<item> result(new item(
            "cylinder_01", transformer_builder.shared_transformer()));
        return result;
    }
    shared_ptr<d3d9::rendering::item> create_bone_item(
        d3d9::graphics_device& device,
        shared_ptr<d3d9::rendering::transformation> transformation)
    {
        using namespace d3d9::rendering;
        transformer_builder transformer_builder(transformation);

        shared_ptr<item> result(new item(
            "bone_01", transformer_builder.shared_transformer()));
        result->set_geometry(
            detail::build_bone(device, 2.0f, vector3f(1, 1, 1)));

        return result;
    }
    detail::deformer_collection build_deformers(
        d3d9::graphics_device& device,
        shared_ptr<d3d9::rendering::item> parent)
    {
        detail::deformer_collection deformers;

        using namespace pt::d3d9::rendering;

        for (size_t i = 0; i < 5; ++i)
        {
            matrix4x4f m = (i == 0)
                ? rotation_matrix(vector3f(1, 0, 0), 3.1415f * 0.5f)
                : translation_matrix(vector3f(0, 0, 2.0f));

            shared_ptr<simple_transformation> bone_transformation(
                new simple_transformation(m));

            shared_ptr<item> item
                = create_bone_item(device, bone_transformation);

            detail::deformer d;
            d.set_item(item);
            d.set_parent_to_rest(m);
            deformers.push_back(d);

            parent->push_back(item);
            parent = item;
        }
        return deformers;
    }
} // anonymous namespace

kimmo_cylinder_deform::kimmo_cylinder_deform(d3d9::graphics_device& device,
    config_node const& config)
:   simple_leaf_sequence(config)
,   d3d9::device_reference(device)
{
    assert(config.key() == "kimmo_cylinder_deform");
    config.parse_child("name", m_name);

    m_transformation.reset(new d3d9::rendering::simple_transformation(
        translation_matrix(vector3f(0, 0, 20))));

    m_item = create_cylinder_item(m_transformation);

    detail::deformer_collection deformers = build_deformers(device, m_item);

    m_item->set_geometry(detail::build_cylinder(device, deformers, m_item));

    m_cylinder_program.reset(new d3d9::rendering::shading_program(
        device, make_shared_ptr(new cylinder_surface_shader)));

    m_bone_program.reset(new d3d9::rendering::shading_program(
        device, make_shared_ptr(new bone_surface_shader)));

    m_variables.reset(new variables);
    m_variables->set_world_to_view_clip(create_projection());
}

std::string const& kimmo_cylinder_deform::name() const
{
    return m_name;
}

namespace {
    void draw_bones(pt::d3d9::rendering::item const& item,
        std::vector<d3d9::rendering::variable_owner const*> const& owners,
        shared_ptr<d3d9::rendering::shading_program> program,
        float time)
    {
        using namespace pt::d3d9::rendering;

        std::vector<variable_owner const*> new_owners = owners;
        new_owners.push_back(item.transformer()->variables().get());

        program->prepare(new_owners, time);
        program->use();

        item.geometry()->drawable()->draw();

        for (item::const_iterator i = item.begin(); i != item.end(); ++i)
        {
            draw_bones(**i, owners, program, time);
        }
    }
} // anonymous namespace
namespace {
    void update_bones(pt::d3d9::rendering::item& item,
        std::vector<d3d9::rendering::variable_owner*> const& owners,
        shared_ptr<d3d9::rendering::shading_program> program)
    {
        using namespace pt::d3d9::rendering;

        std::vector<variable_owner*> new_owners = owners;
        new_owners.push_back(item.transformer()->variables().get());

        program->create_resources(new_owners);


        for (item::const_iterator i = item.begin(); i != item.end(); ++i)
        {
            update_bones(**i, owners, program);
        }
    }
}

shared_ptr<d3d9::texture_2d> kimmo_cylinder_deform::draw(
    rendertarget& target, float time)
{
    {
    m_time = time;

    using namespace pt::d3d9::rendering;

    std::vector<d3d9::rendering::variable_owner*> owners;
    if (view())
        owners.push_back(view()->variables().get());
    //else
        owners.push_back(m_variables.get());

    {
        std::vector<d3d9::rendering::variable_owner*> new_owners = owners;
        new_owners.push_back(m_item->transformer()->variables().get());

        m_cylinder_program->create_resources(new_owners);
    }

    for (item::const_iterator i = m_item->begin(); i != m_item->end(); ++i)
    {
        update_bones(**i, owners, m_bone_program);
    }
    }

    // Draw
    sequences::scoped_optional_rendertarget scoped_target(target);

    d3d9::graphics_device& device = m_cylinder_program->device();
    //d3d9::scoped_render_state fill_mode(device, D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
    d3d9::scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
    d3d9::scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);

    {
        // Draw cylinder.
        std::vector<d3d9::rendering::variable_owner const*> owners;
        owners.push_back(m_item->transformer()->variables().get());

        if (view())
            owners.push_back(view()->variables().get());
        //else
            owners.push_back(m_variables.get());

        m_cylinder_program->prepare(owners, m_time);
        m_cylinder_program->use();

        m_item->geometry()->drawable()->draw();
    }

    // Draw deformers.
    using namespace pt::d3d9::rendering;

    std::vector<variable_owner const*> owners;
    if (view())
        owners.push_back(view()->variables().get());
    else
        owners.push_back(m_variables.get());

    for (item::const_iterator i = m_item->begin(); i != m_item->end(); ++i)
    {
        draw_bones(**i, owners, m_bone_program, m_time);
    }

    return shared_ptr<d3d9::texture_2d>();
}

void kimmo_cylinder_deform::parse_inputs(config_node const&)
{
}

}} // namespace content::sequences
#endif
