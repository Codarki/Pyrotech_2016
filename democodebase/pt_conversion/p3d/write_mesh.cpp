#include "write_mesh.h"
#include "../file_3ds/mesh.h"
#include "p3d_format/chunk_writer.h"
#include "p3d_format/id.h"
#include "p3d_format/version.h"
#include "pt/math_utils.h"
#include "pt/matrix_operations.h"
#include "pt/matrix_utility.h"
#include "pt/file_output_stream.h"

namespace pt { namespace conversion { namespace p3d {

namespace {
    vector3f rh_to_lh(vector3f const& v)
    {
        // Flip y and z elements.
        return vector3f(v[0], v[2], v[1]);
        //return vector3f(v[0], v[1], v[2]);
    }

    vector4f rh_to_lh(vector4f const& v)
    {
        // Flip y and z elements.
        return vector4f(v[0], v[2], v[1], v[3]);
        //return vector4f(v[0], v[1], v[2], v[3]);
    }

    matrix4x4f rh_to_lh(matrix4x4f const& m)
    {
        // Flip y and z elements.
        vector4f col0 = rh_to_lh(m.column(0));
        vector4f col1 = rh_to_lh(m.column(1));
        vector4f col2 = rh_to_lh(m.column(2));
        vector4f col3 = rh_to_lh(m.column(3));

        // Flip y and z basic axises.
        matrix4x4f m2;
        m2.set_column(0, col0);
        m2.set_column(1, col2);
        m2.set_column(2, col1);
        m2.set_column(3, col3);
        /*m2.set_column(0, col0);
        m2.set_column(1, col1);
        m2.set_column(2, col2);
        m2.set_column(3, col3);*/
        return m2;
    }

    std::string optional_base_for_mesh_old(std::string const& mesh,
        std::string const& base)
    {
        std::string const numbers("0123456789");

        std::string base_mesh = base;
        std::string::size_type p3d_model_pos
            = base_mesh.find(".p3d_model");
        if (p3d_model_pos != std::string::npos)
        {
            base_mesh = base_mesh.substr(0, p3d_model_pos);
        }


        std::string::size_type base_first_number_pos
            = base_mesh.find_first_of(numbers);
        std::string::size_type base_last_number_pos
            = base_mesh.find_last_of(numbers);
        if (base_first_number_pos == std::string::npos)
            return std::string();

        // Extract mesh number text positions.

        std::string::size_type mesh_first_number_pos
            = mesh.find_first_of(numbers);
        std::string::size_type mesh_last_number_pos
            = mesh.find_last_of(numbers);
        if (mesh_first_number_pos == std::string::npos)
            return std::string();
        assert(mesh_last_number_pos != std::string::npos);

        // Check for match.
        std::string const stripped_mesh_name
            = mesh.substr(0, mesh_first_number_pos);
        std::string::size_type match_pos = base_mesh.find(stripped_mesh_name);
        if (match_pos == std::string::npos)
            return std::string();

        // Get mesh numbers.
        size_t const number_count
            = mesh_last_number_pos - mesh_first_number_pos + 1;
        size_t const base_number_count
            = base_last_number_pos - base_first_number_pos + 1;
        std::string const mesh_number_text
            = mesh.substr(mesh_first_number_pos, number_count);
        std::string const base_number_text
            = base_mesh.substr(base_first_number_pos, base_number_count);

        int mesh_number = lexical_cast<int>(mesh_number_text);
        int base_number = lexical_cast<int>(base_number_text);

        if (mesh_number <= base_number)
            return std::string();

        return base;
    }

    int parse_number(std::string const& text,
        std::string::size_type first_number_pos,
        std::string::size_type last_number_pos)
    {
        size_t const number_count = last_number_pos - first_number_pos + 1;
        std::string const number_text
            = text.substr(first_number_pos, number_count);
        return boost::lexical_cast<int>(number_text);
    }

    int parse_base_mesh_number(std::string const& base)
    {
        std::string const numbers("0123456789");

        std::string base_mesh = base;

        std::string::size_type base_first_number_pos
            = base_mesh.find_first_of(numbers);
        std::string::size_type base_last_number_pos
            = base_mesh.find_last_of(numbers);
        if (base_first_number_pos == std::string::npos)
        {
            // Base mesh contains no index.
            return -1;
        }

        int base_number = parse_number(base_mesh,
            base_first_number_pos,
            base_last_number_pos);

        return base_number;
    }

    std::string optional_base_for_mesh(std::string const& mesh,
        std::string const& base)
    {
        if (mesh[0] != base[0])
            return std::string();

        std::string const numbers("0123456789");

        std::string base_mesh = base;
        std::string::size_type p3d_model_pos
            = base_mesh.find(".p3d_model");
        if (p3d_model_pos != std::string::npos)
        {
            base_mesh = base_mesh.substr(0, p3d_model_pos);
        }

        // Extract mesh number text positions.

        std::string::size_type mesh_first_number_pos
            = mesh.find_first_of(numbers);
        std::string::size_type mesh_last_number_pos
            = mesh.find_last_of(numbers);
        if (mesh_first_number_pos == std::string::npos)
            return std::string();
        assert(mesh_last_number_pos != std::string::npos);

        // Check for match.
        std::string const stripped_mesh_name
            = mesh.substr(0, mesh_first_number_pos);
        std::string::size_type match_pos = base_mesh.find(stripped_mesh_name);
        if (match_pos == std::string::npos)
            return std::string();

        // Get mesh numbers.
        int base_number = parse_base_mesh_number(base_mesh);

        int mesh_number = parse_number(mesh,
            mesh_first_number_pos,
            mesh_last_number_pos);

        if (mesh_number <= base_number)
            return std::string();

        return base;
    }

    std::string optional_base_mesh_filename(file_3ds::mesh const& mesh,
        config_node const& config)
    {
        if (config.begin() == config.end())
            return std::string();

        config_node::const_iterator asset_iter
            = config.find_child("asset");
        if (asset_iter == config.end())
            return std::string();

        config_node::const_iterator convert_iter
            = asset_iter->find_child("convert");
        if (convert_iter == asset_iter->end())
            return std::string();

        config_node::const_iterator scene_iter
            = convert_iter->find_child("scene");
        if (scene_iter == convert_iter->end())
            return std::string();

        config_node::const_iterator mesh_instances_iter
            = scene_iter->find_child("mesh_instances");
        if (mesh_instances_iter == scene_iter->end())
            return std::string();

        for (config_node::const_iterator i = mesh_instances_iter->begin();
            i != mesh_instances_iter->end(); ++i)
        {
            if (i->key() == "base_mesh")
            {
                std::string arr = optional_base_for_mesh(
                    mesh.name(), i->child("file")->value());

                if (!arr.empty())
                    return arr;
            }
        }
        return std::string();
    }

} // anonymous namespace

std::string get_mesh_filename(boost::shared_ptr<file_3ds::object> const& obj,
    config_node const& config)
{
	boost::shared_ptr<file_3ds::mesh> mesh =
        boost::dynamic_pointer_cast<file_3ds::mesh>(obj);
    assert(mesh);

    std::string tmp = optional_base_mesh_filename(*mesh, config);

    std::string mesh_filename;
    if (tmp.empty())
        mesh_filename = mesh->name() + ".p3d_model";
    else
        mesh_filename = tmp;

    return mesh_filename;
}

void write_scene_mesh(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s, path const& object_directory,
    std::map<std::string,std::string> const& mesh_names)
{
	boost::shared_ptr<file_3ds::mesh> mesh =
        boost::dynamic_pointer_cast<file_3ds::mesh>(obj);
    assert(mesh);

    typedef std::map<std::string,std::string> mesh_to_filename_map;
    mesh_to_filename_map::const_iterator i = mesh_names.find(mesh->name());
    assert(i != mesh_names.end());

    std::string mesh_filename = i->second;

    //std::string tmp = optional_base_mesh_filename(*mesh, config);
    //std::string mesh_filename;
    //if (tmp.empty())
    //    mesh_filename = mesh->name() + ".p3d_model";
    //else
    //    mesh_filename = tmp;

    path mesh_path = object_directory / mesh_filename;

    write(s, static_cast<uint32>(p3d_format::id_mesh));
    write(s, static_cast<uint32>(0)); // chunk_len unkown at this point.

    write(s, mesh->name());
    write(s, mesh_path.string());
    write(s, rh_to_lh(mesh->matrix()));

    // Write mesh material name.
    size_t face_material_count = mesh->face_materials().size();

    // Use only first material for multimaterial meshes.
    if (face_material_count > 1)
        face_material_count = 1;

    write(s, static_cast<uint32>(face_material_count));
    if (face_material_count > 0)
        write(s, mesh->face_materials()[0].mat_name);
}

void write_mesh_model(boost::shared_ptr<file_3ds::object> const& obj,
    file_output_stream& s)
{
    p3d_format::chunk_writer chunk(s, p3d_format::id_mesh_model);
	boost::shared_ptr<file_3ds::mesh> mesh =
        boost::dynamic_pointer_cast<file_3ds::mesh>(obj);
    assert(mesh);

    // Write model header.
    chunk.write<uint16>(p3d_format::version_model);
    for (size_t i = 0; i < 22; ++i)
        chunk.write<uint8>(0);

    size_t vertex_count = mesh->vertices().size();
    size_t face_count = mesh->faces().size();
    size_t face_material_count = mesh->face_materials().size();
    //size_t smoothing_group_count = mesh->face_materials().size();

    chunk.write(mesh->name());
    chunk.write(static_cast<uint32>(vertex_count));
    chunk.write(static_cast<uint32>(face_count));
    chunk.write(static_cast<uint32>(face_material_count));

    // Get left handed inverted local-transformation matrix.
    matrix4x4f lh_mat = rh_to_lh(mesh->matrix());
    matrix4x4f inv_lh_mat = invert(lh_mat);

    // Sanity check, inv_M * M == identity.
    matrix4x4f identity = inv_lh_mat * lh_mat;
    assert(within_denormal(identity(0,0) - 1.0f));
    assert(within_denormal(identity(1,1) - 1.0f));
    assert(within_denormal(identity(2,2) - 1.0f));
    assert(within_denormal(identity(3,3) - 1.0f));

    float radius = 0.0f;
    for (size_t i = 0; i < vertex_count; ++i)
    {
        vector3f lh_position = rh_to_lh(mesh->vertices()[i]);
        vector3f local = transform_position(lh_position, inv_lh_mat);
        radius = std::max(radius, length(local));
    }

    chunk.write(radius);

    for (size_t i = 0; i < vertex_count; ++i)
    {
        vector3f lh_position = rh_to_lh(mesh->vertices()[i]);
        vector3f local = transform_position(lh_position, inv_lh_mat);
        //chunk.write(local);

        // 0:16 fixed point position for range [0,1], multiplied with radius.
        uint16 position_x = static_cast<uint16>((local[0] / radius) * 32767.0f + 32768); 
        uint16 position_y = static_cast<uint16>((local[1] / radius) * 32767.0f + 32768); 
        uint16 position_z = static_cast<uint16>((local[2] / radius) * 32767.0f + 32768); 

        chunk.write(position_x);
        chunk.write(position_y);
        chunk.write(position_z);

        if (mesh->texcoords().empty())
        {
            chunk.write(vector2f(0, 0));
        }
        else
        {
            chunk.write(vector2f(mesh->texcoords()[i]));
        }
    }

    if (vertex_count > 255)
    {
        for (size_t i = 0; i < face_count; ++i)
        {
            chunk.write(file_3ds::mesh::face_type(mesh->faces()[i]));
        }
    }
    else
    {
        // 4 byte optimized version. 
        typedef vector<unsigned char, 4> face_type;

        for (size_t i = 0; i < face_count; ++i)
        {
            face_type face;
            face[0] = static_cast<unsigned char>(mesh->faces()[i][0]);
            face[1] = static_cast<unsigned char>(mesh->faces()[i][1]);
            face[2] = static_cast<unsigned char>(mesh->faces()[i][2]);
            face[3] = static_cast<unsigned char>(mesh->faces()[i][3]);

            chunk.write(face);
        }
    }

    chunk.write(bool(face_material_count > 1));
    for (size_t i = 0; i < face_material_count; ++i)
    {
        file_3ds::mesh::face_mat const& material = mesh->face_materials()[i];
        chunk.write(material.mat_name);
        if (face_material_count > 1)
        {
            chunk.write(static_cast<uint32>(material.face_indices.size()));

            for (size_t j = 0; j < material.face_indices.size(); ++j)
            {
                chunk.write(static_cast<uint16>(material.face_indices[j]));
            }
        }
    }

    chunk.write(bool(mesh->per_face_smoothing_groups()));
    if (mesh->per_face_smoothing_groups())
    {
        assert(mesh->face_smoothing_groups().size() == face_count);

        for (size_t i = 0; i < face_count; ++i)
        {
            chunk.write(static_cast<uint32>(
                mesh->face_smoothing_groups()[i]));
        }
    }

    // write back the size of this chunk
    //size_t current_pos = s.current_pos();
    //size_t chunk_size = current_pos - start_pos;
    //s.seek(chunk_len_offset);
    //pt::write<uint32>(s, static_cast<uint32>(chunk_size));
    //s.seek(current_pos);
}

}}} // namespace pt::conversion::p3d
