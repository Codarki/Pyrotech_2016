#include "scene.h"
#include "animations.h"
#include "camera.h"
#include "chunk_reader.h"
#include "id.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "readers.h"
#include "version.h"
#include "pt/basic_types.h"
#include "pt/any_error.h"
#include "pt/progress_report.h"
#include "pt/vector_utility.h"
#include "pt/format.h"

namespace p3d_format {

using ::boost::shared_ptr;

// keyframes

keyframes::keyframes(chunk_reader& chunk)
{
    m_frame_start = chunk.read<uint32>();
    m_frame_end = chunk.read<uint32>();
    size_t node_count = chunk.read<uint32>();

    for (size_t i = 0; i < node_count; ++i)
    {
        m_animations.push_back(shared_ptr<animation>(
            new animation(chunk)));
    }

    set_parent_hierarchy(m_animations);
}

keyframes::const_iterator keyframes::begin() const
{
    return m_animations.begin();
}

keyframes::const_iterator keyframes::end() const
{
    return m_animations.end();
}

// scene

scene::scene(chunk_reader& chunk, pt::config_node config,
    progress_report* progress, bool only_serialize)
: m_chunk(chunk)
, m_config(config)
{
    if (m_chunk.id() != id_scene)
        throw any_error("not a .p3d_scene");

    check_version();

    while(!chunk.end())
    {
        chunk_reader child(chunk);

        switch(child.id())
        {
        case id_mesh:
            m_mesh_references.push_back(
                shared_ptr<mesh_reference>(new mesh_reference(child)));
            break;

        case id_material:
            m_materials.push_back(shared_ptr<material>(new material(child)));
            break;

        case id_camera:
            m_cameras.push_back(shared_ptr<camera>(new camera(child)));
            break;

        case id_light:
            m_lights.push_back(shared_ptr<light>(new light(child)));
            break;

        case id_animation:
            assert(!m_keyframes);
            m_keyframes.reset(new keyframes(child));
            break;

        default:
            child.skip();
        }

        if (progress)
            progress->advance();
    }

    //typedef std::map<std::string,shared_ptr<mesh> mesh_cache;
    //mesh_cache meshes;

    if (only_serialize)
    {
        // Load meshes referenced by this scene.
        for (std::vector<shared_ptr<mesh_reference> >::const_iterator i
            = m_mesh_references.begin(); i != m_mesh_references.end(); ++i)
        {
            mesh_reference const& mesh_reference = **i;
            shared_ptr<serialized_mesh> serialized(new serialized_mesh(mesh_reference));
            m_serialized_meshes.push_back(serialized);
        }
    }
    else
    {
        // Load meshes referenced by this scene.
        for (std::vector<shared_ptr<mesh_reference> >::const_iterator i
            = m_mesh_references.begin(); i != m_mesh_references.end();)
        {
            try
            {
                //mesh_cache::const_iterator

                path model_path((*i)->filename());

                shared_ptr<mesh> mesh = read_mesh(model_path, progress);
                mesh->set_name((*i)->name());
                mesh->set_default_pose((*i)->matrix());

                m_meshes.push_back(mesh);
                ++i;
            }
            catch (p3d_format::topology_building_error& e)
            {
                pt::log(pt::format("Ignoring mesh %1%: %2%") % (*i)->filename() % e.what());

                m_rejected_mesh_references.push_back(*i);

                // Remove failed mesh from mesh references.
                i = m_mesh_references.erase(i);
            }
        }
    }

    // Assign animation.
    for (keyframes::const_iterator i = m_keyframes->begin();
        i != m_keyframes->end(); ++i)
    {
        shared_ptr<animation> const& animation = *i;

        // Reject animations for rejected meshes.
        bool rejected = false;
        for (std::vector<shared_ptr<mesh_reference> >::iterator iter = m_rejected_mesh_references.begin();
            iter != m_rejected_mesh_references.end() && !rejected; ++iter)
        {
            if ((*iter)->name() == animation->object_name())
            {
                rejected = true;
            }
        }
        if (rejected)
            continue;

        item& item = find_item(animation->object_name());

        if (animation->is_target())
            item.set_target_animation(animation);
        else
            item.set_animation(animation);
    }
}

pt::config_node const& scene::config() const
{
    return m_config;
}

std::vector<shared_ptr<camera> > const& scene::cameras() const
{
    return m_cameras;
}

std::vector<shared_ptr<light> > const& scene::lights() const
{
    return m_lights;
}

std::vector<shared_ptr<mesh> > const& scene::models() const
{
    return m_meshes;
}

std::vector<shared_ptr<material> > const& scene::materials() const
{
    return m_materials;
}

std::vector<shared_ptr<serialized_mesh> > const& scene::serialized_meshes() const
{
    return m_serialized_meshes;
}

void scene::check_version()
{
    int16 version = m_chunk.read<int16>();
    if (version != version_scene)
    {
        std::string error = (format("Scene version %1%.%2%, expected %3%.%4%. Update conversion tool and re-convert data.")
            % lexical_cast<std::string>(PT_VERSION_MAJOR(version))
            % lexical_cast<std::string>(PT_VERSION_MINOR(version))
            % lexical_cast<std::string>(PT_VERSION_MAJOR(version_scene))
            % lexical_cast<std::string>(PT_VERSION_MINOR(version_scene))
            ).move_string();

        throw std::exception(error.c_str());
    }
    for (size_t i = 0; i < 22; ++i)
    {
        m_chunk.read<uint8>();
    }
}

item& scene::find_item(std::string const& name)
{
    for (std::vector<shared_ptr<mesh> >::iterator i = m_meshes.begin();
        i != m_meshes.end(); ++i)
    {
        if ((*i)->name() == name)
            return **i;
    }

    for (std::vector<shared_ptr<serialized_mesh> >::iterator i = m_serialized_meshes.begin();
        i != m_serialized_meshes.end(); ++i)
    {
        if ((*i)->name() == name)
            return **i;
    }

    for (std::vector<shared_ptr<light> >::iterator i = m_lights.begin();
        i != m_lights.end(); ++i)
    {
        if ((*i)->name() == name)
            return **i;
    }

    for (std::vector<shared_ptr<camera> >::iterator i = m_cameras.begin();
        i != m_cameras.end(); ++i)
    {
        if ((*i)->name() == name)
            return **i;
    }

    throw any_error(format("item '%1%' not found") % name);
}

} // namespace p3d_format
