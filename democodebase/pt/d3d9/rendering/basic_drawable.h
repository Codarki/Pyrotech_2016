#ifndef PT_D3D9_RENDERING_BASIC_DRAWABLE_H
#define PT_D3D9_RENDERING_BASIC_DRAWABLE_H

#include "drawable.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
    class index_buffer;
    class vertex_streams;
}}

namespace pt { namespace d3d9 { namespace rendering {

class basic_drawable : public drawable
{
public:
    basic_drawable(
		std::shared_ptr<vertex_streams> streams,
        std::unique_ptr<index_buffer>&& indices);
    ~basic_drawable();

public: // interface required by drawable
    void draw() const;

private: // data members
	std::shared_ptr<vertex_streams> m_streams;
    std::unique_ptr<index_buffer> m_indices;
};

}}} // namespace pt::d3d9::rendering

#endif
