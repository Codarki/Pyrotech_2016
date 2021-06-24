#include "resource.h"
#include "graphics_device.h"

namespace pt { namespace d3d9 {

resource::resource(graphics_device& device)
:   device_child(device)
{
}

}} // namespace pt::d3d9
