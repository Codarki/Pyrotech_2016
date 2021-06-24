#ifndef PT_D3D9_GRAPHICS_SYSTEM_H
#define PT_D3D9_GRAPHICS_SYSTEM_H

#define D3D_DEBUG_INFO

#include "pt/boost/noncopyable.h"
#include "native.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {

class graphics_adapter;

/// Singleton class for graphics system. This class handles all the graphics
/// adapters in system. \note Inserting adapters on the fly does not get
/// this class updated.
class graphics_system : boost::noncopyable
{
public:
    graphics_system();
    ~graphics_system();

    //static bool has_instance();
    //static graphics_system& instance();

    //void create_device(size_t adapter_index, int width, int height,
    //    int bits, HWND handle, bool fullscreen);

	std::shared_ptr<graphics_adapter const> adapter(size_t index) const;
	std::shared_ptr<graphics_adapter> adapter(size_t index);

    com_ptr<IDirect3D9> native();

private: // data members
    com_ptr<IDirect3D9> m_d3d;
	std::vector<std::shared_ptr<graphics_adapter> > m_adapters;
};

}} // namespace pt::d3d9

#endif
