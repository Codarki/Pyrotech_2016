#include "drawable_builder.h"
#include "basic_drawable.h"
#include "pt/d3d9/index_buffer.h"
#include "pt/d3d9/vertex_streams.h"
#include "pt/boost/cast.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    std::unique_ptr<index_buffer> create_index_buffer(graphics_device& device,
        std::vector<size_t> const& indices)
    {
        size_t count = indices.size();
        UINT bytes = static_cast<UINT>(count * sizeof(uint16));
        std::unique_ptr<index_buffer> buffer = create_default_16bit_unique_index_buffer(device, bytes);

        scoped_index_buffer_lock lock(*buffer);

        uint16* dst = lock.data();
        for (size_t i = 0; i < indices.size(); ++i)
        {
            uint16 index = numeric_cast<uint16>(indices[i]);
            *dst++ = index;
        }

        //for (size_t i = 0; i < indices.size(); ++i)
        //{
        //    // Flip the order of triangle vertices so that the system
        //    // traverses them clockwise from the front.
        //    uint16* dst = lock.data() + i * 3;
        //    *dst++ = numeric_cast<uint16>(indices[i][0]);
        //    *dst++ = numeric_cast<uint16>(indices[i][2]);
        //    *dst++ = numeric_cast<uint16>(indices[i][1]);
        //}

        return buffer;
    }
}

std::shared_ptr<basic_drawable> build_shared_basic_drawable(
    graphics_device& device,
	std::shared_ptr<vertex_streams> const& streams,
    std::vector<size_t> const& indices)
{
    streams->finalize();

    auto index_buffer = create_index_buffer(device, indices);

	return std::make_shared<basic_drawable>(
        streams, move(index_buffer));
}

}}} // namespace pt::d3d9::rendering
