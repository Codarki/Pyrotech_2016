#pragma once
#ifndef PT_D3D9_VERTEX_SHADER_H
#define PT_D3D9_VERTEX_SHADER_H

#include "vertex_shader_fwd.h"
#include "shader.h"
#include "native.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {

class vertex_shader : public shader
{
public:
    vertex_shader(graphics_device& device, com_ptr<IDirect3DVertexShader9> native,
        com_ptr<ID3DXConstantTable> constants);
    vertex_shader(vertex_shader const& other);
    ~vertex_shader();

    void use() const;

public: // interface required by shader
    constant_buffer const& constants() const;
    constant_buffer& constants();

protected: // interface required by device_child
    void before_destruction();
    void before_reset();
    void after_reset();

private: // data members
    com_ptr<IDirect3DVertexShader9> m_native;
    std::unique_ptr<constant_buffer> m_constants;
};

}} // namespace pt::d3d9

#endif
