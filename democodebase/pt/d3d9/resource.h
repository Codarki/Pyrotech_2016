#pragma once
#ifndef PT_D3D9_RESOURCE_H
#define PT_D3D9_RESOURCE_H

#include "device_child.h"

namespace pt { namespace d3d9 {

/// Base class for all resources. This is mainly helpful when moving from
/// d3d9 to d3d10.
class resource : public device_child
{
public:
    resource(graphics_device& device);
};

}} // namespace pt::d3d9

#endif
