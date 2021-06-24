#ifndef PT_P3D_FORMAT_SCENE_H
#define PT_P3D_FORMAT_SCENE_H

#include "pt/config_node.h"
#include "pt/vector.h"
#include "pt/boost/noncopyable.h"
#include "pt/boost/shared_ptr.h"
#include "pt/std/vector.h"

namespace pt {
class progress_report;
}

namespace p3d_format {

using namespace pt;

class animation;
class camera;
class chunk_reader;
class item;
class light;
class material;
class mesh;
class mesh_reference;
class serialized_mesh;

class keyframes
{
private: // type definitions
	typedef std::vector<boost::shared_ptr<animation> > container;
public:
    typedef container::const_iterator const_iterator;

public:
    keyframes(chunk_reader& chunk);

    const_iterator begin() const;
    const_iterator end() const;

private: // data members
    size_t m_frame_start;
    size_t m_frame_end;
	std::vector<boost::shared_ptr<animation> > m_animations;
};

class scene : boost::noncopyable
{
public:
    scene(chunk_reader& chunk, pt::config_node config, progress_report* progress, bool only_serialize);

    pt::config_node const& config() const;
	std::vector<boost::shared_ptr<camera> > const& cameras() const;
	std::vector<boost::shared_ptr<light> > const& lights() const;
	std::vector<boost::shared_ptr<mesh> > const& models() const;
	std::vector<boost::shared_ptr<material> > const& materials() const;

	std::vector<boost::shared_ptr<serialized_mesh> > const& serialized_meshes() const;

private: // helper functions
    void check_version();
    item& find_item(std::string const& name);

private: // data members
    chunk_reader& m_chunk;
    pt::config_node m_config;

	std::vector<boost::shared_ptr<mesh_reference> > m_mesh_references;
	std::vector<boost::shared_ptr<mesh_reference> > m_rejected_mesh_references;
	std::vector<boost::shared_ptr<camera> > m_cameras;
	std::vector<boost::shared_ptr<light> > m_lights;
	std::vector<boost::shared_ptr<material> > m_materials;
	std::vector<boost::shared_ptr<mesh> > m_meshes;
	std::vector<boost::shared_ptr<serialized_mesh> > m_serialized_meshes;
	boost::shared_ptr<keyframes> m_keyframes;
};

} // namespace p3d_format

#endif
