#include "draw_rectangle.h"
#include "device_convenience.h"
#include "dynamic_vertex_buffer.h"
#include "scoped_convenience.h"

namespace pt { namespace d3d9 {
namespace {
    struct vertex
    {
        float x, y, z;
    };
} // anonymous namespace

void draw_rectangle(graphics_device& device, box2f const& area,
    vector4f const& color)
{
    draw_rectangle(device, min_position(area), max_position(area), color);
}

void draw_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector4f const& color)
{
    UINT fvf = D3DFVF_XYZ;

    dynamic_vertex_buffer buf(device, sizeof(vertex) * 5, fvf, 0);

    vertex* p = buf.data<vertex>();

    p->x = min[0] - 0.5f;
    p->y = min[1] - 0.5f;
    p->z = 0.0;
    p++;

    p->x = max[0] - 0.5f;
    p->y = min[1] - 0.5f;
    p->z = 0.0f;
    p++;

    p->x = max[0] - 0.5f;
    p->y = max[1] - 0.5f;
    p->z = 0.0f;
    p++;

    p->x = min[0] - 0.5f;
    p->y = max[1] - 0.5f;
    p->z = 0.0f;
    p++;

    p->x = min[0] - 0.5f;
    p->y = min[1] - 0.5f;
    p->z = 0.0;

    buf.use(0, sizeof(vertex));

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);

    D3DXMATRIX projection;

    D3DVIEWPORT9 view = viewport(device);

    D3DXMatrixOrthoOffCenterLH(&projection,
        static_cast<float>(view.X),
        static_cast<float>(view.X + view.Width),
        static_cast<float>(view.Y + view.Height),
        static_cast<float>(view.Y),
        0.0f, 1.0f);

    vector4f color2 = color / 255.0f;
    D3DCOLOR d3d_color = D3DCOLOR_COLORVALUE(color2[0], color2[1], color2[2],
        color2[3]);

    scoped_render_state texture_factor(device, D3DRS_TEXTUREFACTOR,
        d3d_color);
    scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
    scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
    scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);
    //scoped_render_state alpha_blend(D3DRS_ALPHABLENDENABLE, FALSE);

    scoped_texture_stage_state color_op(device, 0, D3DTSS_COLOROP,
        D3DTOP_SELECTARG1);
    scoped_texture_stage_state color_arg1(device, 0, D3DTSS_COLORARG1,
        D3DTA_TFACTOR);

    set_transform(device, D3DTS_WORLD, &identity);
    set_transform(device, D3DTS_VIEW, &identity);
    set_transform(device, D3DTS_PROJECTION, &projection);

    if (color[3] < 255.0f)
    {
        scoped_render_state alpha_blend(device, D3DRS_ALPHABLENDENABLE,
            TRUE);
        scoped_render_state src_blend(
            device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        scoped_render_state dst_blend(
            device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        scoped_texture_stage_state alpha_op(
            device, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        scoped_texture_stage_state alpha_arg1(
            device, 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

        draw_primitive(device, D3DPT_LINESTRIP, 0, 4);
    }
    else
    {
        scoped_render_state alpha_blend(device, D3DRS_ALPHABLENDENABLE,
            FALSE);
        
        draw_primitive(device, D3DPT_LINESTRIP, 0, 4);
    }
}

}} // namespace pt::d3d9
