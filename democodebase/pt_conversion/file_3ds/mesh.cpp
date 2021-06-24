#include "mesh.h"
#include "common.h"
#include "pt/bounded_input_stream.h"
#include "pt/box_builder.h"
#include "pt/log.h"
#include "pt/vector_utility.h"
#include "pt/matrix_operations.h"
#include "pt/string_wrapper.h"
#include "pt/boost/format.h"

namespace pt { namespace conversion { namespace file_3ds {

mesh::mesh(std::string const& name)
:   object(name)
,   m_per_face_smoothing_groups(false)
{
}

void mesh::read(chunk_info const& parent_info, bounded_input_stream& s)
{
    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_VERTLIST))
        {
            log(str(boost::format("vertices (%1$x)") % info.id));
            read_vertices(s);
        }
        else if (info.is(CHUNK_FACELIST))
        {
            log(str(boost::format("facelist (%1$x)") % info.id));
            read_faces(s);

            read_face_subchunks(info, s);
        }
        else if (info.is(CHUNK_MAPLIST))
        {
            log(str(boost::format("maplist (%1$x)") % info.id));
            read_texcoords(s);
        }
        else if (info.is(CHUNK_TRMATRIX))
        {
            log(str(boost::format("trmatrix (%1$x)") % info.id));
            read_matrix(s);
        }
        else if (info.is(CHUNK_MESHCOLOR))
        {
            // mesh color in editor
            //log(format("meshcolor (%1$x)") % info.id);
        }
        else if (info.is(CHUNK_MESHTEXTUREINFO))
        {
            log(str(boost::format("meshtextureinfo (%1$x)") % info.id));
        }
        else
        {
            log(str(boost::format("unknown (%1$x)") % info.id));
        }

        skip_chunk(info, s);
    }
}

mesh::vertex_container const& mesh::vertices() const
{
    return m_vertices;
}

mesh::face_container const& mesh::faces() const
{
    return m_faces;
}

mesh::face_material_container const& mesh::face_materials() const
{
    return m_face_materials;
}

mesh::face_smoothing_group_container const& mesh::face_smoothing_groups() const
{
    return m_face_smoothing_groups;
}

mesh::texcoord_container const& mesh::texcoords() const
{
    return m_texcoords;
}

matrix4x4f const& mesh::matrix() const
{
    return m_matrix;
}

bool mesh::per_face_smoothing_groups() const
{
    return m_per_face_smoothing_groups;
}

/*
unsigned int mesh::smoothing_group() const
{
    assert(!m_per_face_smoothing_groups);
    if (face_smoothing_groups().size() > 0)
        return face_smoothing_groups()[0];
    return 0;
}
*/

void mesh::read_vertices(bounded_input_stream& s)
{
    size_t vertex_count = pt::read<unsigned short>(s);
	
    m_vertices.reserve(vertex_count);

    box_builder3f builder;

    for (size_t i = 0; i < vertex_count; ++i)
    {
        vector3f v = pt::read<vector3f>(s);
        builder.add(v);
        m_vertices.push_back(v);
    }

    if (!builder.empty())
    {
        box3f box = builder.bounding_box();

        log(str(boost::format("bounding box "
            "center (%3.2f %=3.2f %=3.2f) "
            "size (%3.2f %=3.2f %=3.2f)")
            % box.center()[0] % box.center()[1] % box.center()[2]
            % box.size()[0] % box.size()[1] % box.size()[2]));
    }
}

void mesh::read_faces(bounded_input_stream& s)
{
    size_t face_count = pt::read<unsigned short>(s);
    m_faces.reserve(face_count);

    for (size_t i = 0; i < face_count; ++i)
    {
        face_type f = pt::read<face_type>(s);
        m_faces.push_back(f);
    }
/*  Face flag
    bit 0 : CA visible
    bit 1 : BC visible
    bit 2 : AB visible*/
}

void mesh::read_texcoords(bounded_input_stream& s)
{
    if (!m_texcoords.empty())
    {
        log("Multiple UV sets (texcoords).");
    }
    size_t texcoord_count = pt::read<unsigned short>(s);
    m_texcoords.reserve(texcoord_count);

    for (size_t i = 0; i < texcoord_count; ++i)
    {
        // Looks like v is upside down.
        vector2f t = pt::read<vector2f>(s);
        t[1] = 1.0f - t[1];
        m_texcoords.push_back(t);
    }
}

void mesh::read_matrix(bounded_input_stream& s)
{
    m_matrix.set_row(0, vector4f(pt::read<vector3f>(s)));
    m_matrix.set_row(1, vector4f(pt::read<vector3f>(s)));
    m_matrix.set_row(2, vector4f(pt::read<vector3f>(s)));
    m_matrix.set_row(3, vector4f(0, 0, 0, 1));

    vector3f origo = pt::read<vector3f>(s);
    matrix4x4f translation = translation_matrix(origo);

    m_matrix = m_matrix * translation;

    //vector3f scaling = denormalize(basis_axis_lengths(m_matrix));

    //m_matrix = pt::orthogonalize(m_matrix);
    //denormalize_in_place(m_matrix);
    //m_matrix = m_matrix;

    matrix4x4f m2 = m_matrix;
    denormalize_in_place(m2);
    for (size_t i = 0; i < 4; ++i)
    {
        log(str(boost::format("%=8.2f %=8.2f %=8.2f")
            % m2(i, 0)
            % m2(i, 1)
            % m2(i, 2)));
    }
}

void mesh::read_face_material_list(bounded_input_stream& s)
{
    face_mat mat;
    mat.mat_name = read_c_str(s);

    // number of faces affected by this material
    size_t entry_count = pt::read<uint16>(s);

    // indices of the faces
    mat.face_indices.reserve(entry_count);
    for (size_t i = 0; i < entry_count; ++i)
    {
        mat.face_indices.push_back(pt::read<uint16>(s));
    }
    m_face_materials.push_back(mat);
}

void mesh::read_face_smoothing_groups(bounded_input_stream& s)
{
    size_t face_count = m_faces.size();
    m_face_smoothing_groups.reserve(face_count);

    // the nth bit indicate if the face belongs or not to the nth smoothing
    // group

    uint32 previous_sg = 0;
    for (size_t i = 0; i < face_count; ++i)
    {
        uint32 sg = pt::read<uint32>(s);
        m_face_smoothing_groups.push_back(sg);

        if (i > 0 && previous_sg != sg)
        {
            m_per_face_smoothing_groups = true;
        }
        previous_sg = sg;
    }
}

void mesh::read_face_subchunks(chunk_info const& parent_info,
    bounded_input_stream& s)
{
    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.id == 0 || info.end_offset > parent_info.end_offset)
        {
            assert(false);
            return;
        }

        if (info.is(CHUNK_FACEMAT))
        {
            read_face_material_list(s);
        }
        else if (info.is(CHUNK_SMOOLIST))
        {
            read_face_smoothing_groups(s);
        }

        skip_chunk(info, s);
    }
}   

boost::shared_ptr<mesh> read_mesh(chunk_info const& parent_info,
    std::string const& name, bounded_input_stream& stream)
{
    //enable_verbose(false);

	boost::shared_ptr<mesh> m(new mesh(name));
    m->read(parent_info, stream);

    //for (auto i : m->face_materials())
    //{
    //    // Ignore all meshes with material name "pt_ignore".
    //    auto name = i.mat_name;
    //    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    //    if (name == "pt_ignore")
    //        return nullptr;
    //}

    //enable_verbose(true);
    return m;
}

}}} // namespace pt::conversion::file_3ds
