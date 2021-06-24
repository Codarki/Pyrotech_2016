#ifndef CONTENT_SPLINE_TEST_H
#define CONTENT_SPLINE_TEST_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
    class vertex_buffer;
}}

namespace content {

using namespace pt;

class spline_test
{
public:
    spline_test(d3d9::graphics_device& device);

    void draw() const;
    void update(float time);

private: // data members
	std::unique_ptr<d3d9::vertex_buffer> m_vertex_buffer;
};

} // namespace content

#endif
