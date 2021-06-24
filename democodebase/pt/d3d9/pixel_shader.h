#pragma once
#ifndef PT_D3D9_PIXEL_SHADER_H
#define PT_D3D9_PIXEL_SHADER_H

#include "pixel_shader_fwd.h"
#include "shader.h"
#include "native.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {

class pixel_shader : public shader
{
public:
    pixel_shader(graphics_device& device, com_ptr<IDirect3DPixelShader9> native,
        com_ptr<ID3DXConstantTable> constants);
    pixel_shader(pixel_shader const& other);
    ~pixel_shader();

    void use() const;

public: // interface required by shader
    constant_buffer const& constants() const;
    constant_buffer& constants();

protected: // interface required by device_child
    void before_destruction();
    void before_reset();
    void after_reset();

private: // data members
    com_ptr<IDirect3DPixelShader9> m_native;
    std::unique_ptr<constant_buffer> m_constants;
};

}} // namespace pt::d3d9

#endif
