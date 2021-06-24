#include "fill_rectangle.h"
#include "device_convenience.h"
#include "dynamic_vertex_buffer.h"
#include "scoped_convenience.h"

namespace pt { namespace d3d9 {
namespace {
    struct vertex
    {
        float x, y, z;
    };
    struct texcoord_vertex
    {
        float x, y, z;
        float u, v;
    };
    void fill(graphics_device& device, vector2f const& min,
        vector2f const& max, vector2f const& min_uv, vector2f const& max_uv)
    {
        UINT fvf = D3DFVF_XYZ | D3DFVF_TEX1;// | D3DFVF_TEXCOORDSIZE2(1);

        dynamic_vertex_buffer buf(device, sizeof(texcoord_vertex) * 4,
            fvf, 0);

        texcoord_vertex* p = buf.data<texcoord_vertex>();

        D3DVIEWPORT9 view = viewport(device);

        float x_offset = 0.5f; //static_cast<float>(1.0 / static_cast<double>(view.Width) * 0.5);
        float y_offset = 0.5f; //static_cast<float>(1.0 / static_cast<double>(view.Height) * 0.5);

        p->x = min[0] - x_offset;
        p->y = min[1] - y_offset;
        p->z = 0.0;
        p->u = min_uv[0];
        p->v = min_uv[1];
        p++;

        p->x = max[0] - x_offset;
        p->y = min[1] - y_offset;
        p->z = 0.0f;
        p->u = max_uv[0];
        p->v = min_uv[1];
        p++;

        p->x = max[0] - x_offset;
        p->y = max[1] - y_offset;
        p->z = 0.0f;
        p->u = max_uv[0];
        p->v = max_uv[1];
        p++;

        p->x = min[0] - x_offset;
        p->y = max[1] - y_offset;
        p->z = 0.0f;
        p->u = min_uv[0];
        p->v = max_uv[1];

        buf.use(0, sizeof(texcoord_vertex));

        D3DXMATRIX identity;
        D3DXMatrixIdentity(&identity);

        D3DXMATRIX projection;

        D3DXMatrixOrthoOffCenterLH(&projection,
            static_cast<float>(view.X),
            static_cast<float>(view.X + view.Width),
            static_cast<float>(view.Y + view.Height),
            static_cast<float>(view.Y),
            0.0f, 1.0f);

        set_transform(device, D3DTS_WORLD, &identity);
        set_transform(device, D3DTS_VIEW, &identity);
        set_transform(device, D3DTS_PROJECTION, &projection);

        scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
        scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
        scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);

        set_vertex_shader(device, com_ptr<IDirect3DVertexShader9>());
        set_pixel_shader(device, com_ptr<IDirect3DPixelShader9>());
        //set_vertex_declaration(device, com_ptr<IDirect3DVertexDeclaration9>());

        draw_primitive(device, D3DPT_TRIANGLEFAN, 0, 2);

        set_fvf(device, 0);
        set_stream_source(device, 0, com_ptr<IDirect3DVertexBuffer9>(),
            0, 0);
    }
} // anonymous namespace

void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, com_ptr<IDirect3DTexture9> texture)
{
    fill_rectangle(device, min, max, vector2f(0, 0), vector2f(1, 1),
        texture, false);
}

void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, com_ptr<IDirect3DTexture9> texture,
    bool alpha_blend)
{
    fill_rectangle(device, min, max, vector2f(0, 0), vector2f(1, 1),
        texture, alpha_blend);
}

void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector2f const& min_uv, vector2f const& max_uv,
    com_ptr<IDirect3DTexture9> texture, bool alpha_blend)
{
    set_texture(device, 0, texture);

    if (alpha_blend)
    {
        scoped_render_state alpha(device, D3DRS_ALPHABLENDENABLE, TRUE);
        scoped_render_state src(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        scoped_render_state dst(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        scoped_texture_stage_state color_op(
            device, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        scoped_texture_stage_state color_arg1(
            device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

        scoped_texture_stage_state alpha_op(
            device, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        scoped_texture_stage_state alpha_arg1(
            device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

        fill(device, min, max, min_uv, max_uv);
    }
    else
    {
        scoped_render_state alpha(device, D3DRS_ALPHABLENDENABLE, FALSE);
        scoped_texture_stage_state color_op(
            device, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        scoped_texture_stage_state color_arg1(
            device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

        fill(device, min, max, min_uv, max_uv);
    }
}

void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector4f const& color,
    com_ptr<IDirect3DTexture9> texture, bool alpha_blend)
{
    set_texture(device, 0, texture);

    vector4f color2 = color / 255.0f;
    D3DCOLOR d3d_color = D3DCOLOR_COLORVALUE(color2[0], color2[1], color2[2],
        color2[3]);

    scoped_render_state texture_factor(device, D3DRS_TEXTUREFACTOR,
        d3d_color);

    scoped_texture_stage_state color_op(
        device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    scoped_texture_stage_state color_arg1(
        device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    scoped_texture_stage_state color_arg2(
        device, 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    if (alpha_blend)
    {
        scoped_render_state alpha(device, D3DRS_ALPHABLENDENABLE, TRUE);

        scoped_texture_stage_state alpha_op(
            device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        scoped_texture_stage_state alpha_arg1(
            device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        scoped_texture_stage_state alpha_arg2(
            device, 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

        fill(device, min, max, vector2f(0, 0), vector2f(1, 1));
    }
    else
    {
        scoped_render_state alpha(device, D3DRS_ALPHABLENDENABLE, FALSE);

        fill(device, min, max, vector2f(0, 0), vector2f(1, 1));
    }
}

void fill_rectangle(graphics_device& device, box2f const& area,
    vector4f const& color)
{
    fill_rectangle(device, min_position(area), max_position(area), color);
}

void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector4f const& color)
{
    UINT fvf = D3DFVF_XYZ;

    dynamic_vertex_buffer buf(device, sizeof(vertex) * 4, fvf, 0);

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

    scoped_render_state tex_factor(device, D3DRS_TEXTUREFACTOR, d3d_color);
    scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
    scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
    scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);
    //scoped_render_state alpha_blend(D3DRS_ALPHABLENDENABLE, FALSE);

    scoped_texture_stage_state color_op(
        device, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    scoped_texture_stage_state color_arg1(
        device, 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

    set_transform(device, D3DTS_WORLD, &identity);
    set_transform(device, D3DTS_VIEW, &identity);
    set_transform(device, D3DTS_PROJECTION, &projection);

    if (color[3] < 255.0f)
    {
        scoped_render_state alpha(device, D3DRS_ALPHABLENDENABLE, TRUE);
        scoped_render_state src(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        scoped_render_state dst(device, D3DRS_DESTBLEND,
            D3DBLEND_INVSRCALPHA);

        scoped_texture_stage_state alpha_op(device, 0, D3DTSS_ALPHAOP,
            D3DTOP_SELECTARG1);
        scoped_texture_stage_state alpha_arg1(device, 0, D3DTSS_ALPHAARG1,
            D3DTA_TFACTOR);

        draw_primitive(device, D3DPT_TRIANGLEFAN, 0, 2);
    }
    else
    {
        scoped_render_state alpha(device, D3DRS_ALPHABLENDENABLE, FALSE);
        
        set_vertex_shader(device, com_ptr<IDirect3DVertexShader9>());
        //set_vertex_declaration(device, com_ptr<IDirect3DVertexDeclaration9>());
        set_fvf(device, fvf);

        draw_primitive(device, D3DPT_TRIANGLEFAN, 0, 2);

        set_fvf(device, 0);
        set_stream_source(device, 0, com_ptr<IDirect3DVertexBuffer9>(),
            0, 0);
    }
}

void draw_line(graphics_device& device, vector2f const& from,
    vector2f const& to, vector4f const& color)
{
    UINT fvf = D3DFVF_XYZ;

    dynamic_vertex_buffer buf(device, sizeof(vertex) * 2, fvf, 0);

    vertex* p = buf.data<vertex>();

    p->x = from[0] - 0.5f;
    p->y = from[1] - 0.5f;
    p->z = 0.0;
    p++;

    p->x = to[0] - 0.5f;
    p->y = to[1] - 0.5f;
    p->z = 0.0f;
    p++;

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
    scoped_render_state alpha_blend(device, D3DRS_ALPHABLENDENABLE, FALSE);

    scoped_texture_stage_state color_op(device, 0, D3DTSS_COLOROP,
        D3DTOP_SELECTARG1);
    scoped_texture_stage_state color_arg1(device, 0, D3DTSS_COLORARG1,
        D3DTA_TFACTOR);

    set_transform(device, D3DTS_WORLD, &identity);
    set_transform(device, D3DTS_VIEW, &identity);
    set_transform(device, D3DTS_PROJECTION, &projection);

    draw_primitive(device, D3DPT_LINELIST, 0, 1);
}

}} // namespace pt::d3d9
