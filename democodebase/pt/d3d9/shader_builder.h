#ifndef PT_D3D9_SHADER_BUILDER_H
#define PT_D3D9_SHADER_BUILDER_H

#include "native.h"
#include "constant_buffer_fwd.h"
#include "pt/com_ptr.h"

namespace pt { namespace d3d9 {

class shader_builder
{
public:
    shader_builder(graphics_device& device);
    ~shader_builder();

    com_ptr<IDirect3DPixelShader9> build_pixel_shader(DWORD const* function);
    com_ptr<IDirect3DVertexShader9> build_vertex_shader(DWORD const* function);

private: // data members
    graphics_device* m_device;
};

}} // namespace pt::d3d9

#endif
