#ifndef PT_CONVERSION_FILE_3DS_MESH_H
#define PT_CONVERSION_FILE_3DS_MESH_H

#include "object.h"
#include "pt/basic_types.h"
#include "pt/matrix.h"
#include "pt/vector.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/vector.h"

namespace pt {
class bounded_input_stream;
    
namespace conversion { namespace file_3ds {

struct chunk_info;

/// Internalizes a 3ds mesh from a stream.
class mesh : public object
{
public: // type definitions
    /// Maps material to triangle indices.
    struct face_mat
    {
        std::string mat_name;
        std::vector<uint16> face_indices;
    };
    typedef vector<unsigned short, 4> face_type;

    typedef std::vector<vector3f> vertex_container;
    typedef std::vector<face_type> face_container;
    typedef std::vector<face_mat> face_material_container;
    typedef std::vector<uint32> face_smoothing_group_container;
    typedef std::vector<vector2f> texcoord_container;

public:
    mesh(std::string const& name);

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    vertex_container const& vertices() const;
    face_container const& faces() const;
    face_material_container const& face_materials() const;
    face_smoothing_group_container const& face_smoothing_groups() const;
    texcoord_container const& texcoords() const;
    matrix4x4f const& matrix() const;

    /// Returns \c true if faces are divided to multiple smoothing groups.
    bool per_face_smoothing_groups() const;
    //unsigned int smoothing_group() const;

public: // object virtual function implementations
    bool is_mesh() const {return true;}
    bool is_light() const {return false;}
    bool is_camera() const {return false;}

private: // helper functions
    void read_vertices(bounded_input_stream& s);
    void read_faces(bounded_input_stream& s);
    void read_texcoords(bounded_input_stream& s);
    void read_matrix(bounded_input_stream& s);

    void read_face_material_list(bounded_input_stream& s);
    void read_face_smoothing_groups(bounded_input_stream& s);
    void read_face_subchunks(chunk_info const& parent_info,
        bounded_input_stream& s);

private: // data members
    vertex_container m_vertices;
    face_container m_faces;
    face_material_container m_face_materials;
    face_smoothing_group_container m_face_smoothing_groups;
    texcoord_container m_texcoords;
    matrix4x4f m_matrix;

    bool m_per_face_smoothing_groups;
};

/// \return mesh with a \a name internalized from \a stream.
boost::shared_ptr<mesh> read_mesh(chunk_info const& parent_info,
    std::string const& name, bounded_input_stream& stream);

}}} // namespace pt::conversion::file_3ds

#endif
