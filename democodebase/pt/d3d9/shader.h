#pragma once
#ifndef PT_D3D9_SHADER_H
#define PT_D3D9_SHADER_H

#include "device_child.h"
#include "constant_buffer_fwd.h"

namespace pt { namespace d3d9 {

class shader : public device_child
{
public:
    shader(graphics_device& device);
    ~shader();

public: // interface which subclasses must implement
    virtual constant_buffer const& constants() const = 0;
    virtual constant_buffer& constants() = 0;
};

}} // namespace pt::d3d9

#endif
