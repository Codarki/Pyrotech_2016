#ifndef CONTENT_PROGRESS_BAR_H
#define CONTENT_PROGRESS_BAR_H

//#include "pt/d3d9/device_child.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/vector.h"
#include "pt/std/memory.h"

namespace content {

namespace sequences {
    class image;
}

using namespace pt;

class progress_bar
{
public:
    progress_bar(d3d9::graphics_device& device);
    progress_bar(d3d9::graphics_device& device, config_node const& config,
        vector2i resolution);
    ~progress_bar();
    void draw();
    void set_tick(size_t tick);
    void add_tick();

private: // data members
    d3d9::graphics_device* m_device;
	std::shared_ptr<sequences::image> m_mask;
    std::shared_ptr<sequences::image> m_image;
    size_t m_total_ticks;
    size_t m_current_tick;
    vector2i m_min;
    vector2i m_size;
};

} // namespace content

#endif
