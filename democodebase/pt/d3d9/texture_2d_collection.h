#ifndef PT_D3D9_TEXTURE_2D_COLLECTION_H
#define PT_D3D9_TEXTURE_2D_COLLECTION_H

#include "texture_2d_fwd.h"
#include "pt/debug_only.h"
#include "pt/std/cassert.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"
#include "pt/std/map.h"
#include "pt/com_ptr.h"
#include "pt/d3d9/native.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/vector_utility.h"
#include "pt/d3d9/device_convenience.h"

namespace pt { namespace d3d9 {

class texture_2d_collection
{
public: // type definitions
    typedef std::string key_type;

public:
    texture_2d_collection()
    {
    }
    ~texture_2d_collection()
    {
    }

	void add(key_type const& key, std::shared_ptr<texture_2d> texture)
    {
        PT_DEBUG_ONLY(bool inserted =)
            m_textures.insert(map::value_type(key, texture)).second;
        assert(inserted);
    }

	std::shared_ptr<texture_2d> find(key_type const& key)
    {
        map::iterator i = m_textures.find(key);
        if (i != m_textures.end())
            return i->second;

		return nullptr;
    }

    void clear()
    {
        m_textures.clear();
    }

private: // type definitions
	typedef std::map<key_type,std::shared_ptr<texture_2d> > map;

private: // data members
    map m_textures;
};

texture_2d_collection& default_texture_2d_collection();
void clear_default_texture_2d_collection();

com_ptr<IDirect3DSurface9> get_depth_stencil(d3d9::graphics_device& device, pt::vector2i resolution);
void clear_default_depth_stencil();

}} // namespace pt::d3d9

#endif
