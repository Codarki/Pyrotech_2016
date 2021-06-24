#include "post_processing.h"
#include "bloom.h"
#include "blur.h"
#include "dof.h"
#include "light_scattering.h"
#include "bright_pass_filter.h"
#include "bright_pass_filter2.h"
#include "tone_mapping.h"
#include "scale.h"
#include "noise.h"
#include "pt/d3d9/dynamic_vertex_buffer.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/texture_2d_value_utils.h"
#include "pt/d3d9/texture_2d_utils.h"
#include "pt/log.h"
#include "pt/parameter_node_utility.h"
#include "pt/path.h"
#include "pt/format.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    std::shared_ptr<texture_2d> create_render_target(graphics_device& device)
    {
        vector2i resolution(
            device.parameters().BackBufferWidth,
            device.parameters().BackBufferHeight);

        return texture_2d_builder(device)
            .resolution(resolution)
            .levels(1)
            .usage(D3DUSAGE_RENDERTARGET)
            .format(D3DFMT_A16B16G16R16F)
            .pool(D3DPOOL_DEFAULT)
            .build_shared();
    }
}

// post_processing

post_processing::post_processing(graphics_device& device,
    parameter_collection const& parameters)
:   device_reference(device)
,   m_render_target_texture(create_render_target(device))
,   m_current_adapted_luminance(1.0f)
{
    log("Creating post processing effects");
    logging::scoped_indent indent;

    UINT fvf = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(1);
    dynamic_vertex_buffer::ensure_lenght(device, 1024, fvf);

    //if (shared_ptr<parameter_node> node
    //    = optional_child_node("adapted_luminance", parameters))
    //{
    //    m_adapted_luminance = get_animatable<float>(*node);
    //}

    vector2i resolution(
        device.parameters().BackBufferWidth,
        device.parameters().BackBufferHeight);

    //m_tone_mapping.reset(new tone_mapping(device, m_current_adapted_luminance));

    //m_scale.reset(new scale(device, resolution, scale::downscale_4));
    //m_bright_pass.reset(new bright_pass_filter(device, parameters, resolution / 4, D3DFMT_A8R8G8B8));
    //m_blur.reset(new blur(device, resolution / 4, D3DFMT_X8R8G8B8));

    //m_scale_to_bloom_source.reset(
    //    new scale(device, m_blur->resolution(), scale::downscale_2));

    //m_bloom.reset(new bloom(device, *parameters.child("bloom"), resolution / 8, D3DFMT_X8R8G8B8));
	std::shared_ptr<parameter_node const> light_scattering_parameter = *parameters.child("light_scattering");
    std::shared_ptr<parameter_node const> dof_parameter = *parameters.child("depth_of_field");
    std::shared_ptr<parameter_node const> noise_parameter = *parameters.child("noise");

    std::shared_ptr<parameter_node const> bloom_parameter
        = *parameters.child("bloom");
    std::shared_ptr<parameter_node const> bloom_bright_pass_parameter
        = get_child("bright_pass", *bloom_parameter);

    std::shared_ptr<parameter_node const> tone_mapping_parameter
        = *parameters.child("tone_mapping");

    //m_tone_mapping.reset(
    //    new tone_mapping(device, *tone_mapping_parameter));
	m_dof.reset(new dof(device, *dof_parameter, resolution, D3DFMT_A8R8G8B8));

    //Jari: uusi versio
    m_tone_mapping.reset(
        new tone_mapping(device, *tone_mapping_parameter,resolution,D3DFMT_A8R8G8B8));

    m_bright_pass = build_bright_pass_filter(
        device, *bloom_bright_pass_parameter, resolution, D3DFMT_A8R8G8B8);

    //m_bright_pass2.reset(new bright_pass_filter2(
    //    device, *bloom_bright_pass_parameter, resolution, D3DFMT_A8R8G8B8));

    m_scale.reset(
        new scale(device, resolution, scale::downscale_4, D3DFMT_A8R8G8B8));

    auto texture
        = find_or_load("data/images/common/white.tga", device);

    std::shared_ptr<texture_2d_value> blur_strength = create_constant_texture_2d_value(texture);

    m_blur.reset(new blur(device, m_scale->resolution(), D3DFMT_A8R8G8B8, blur_strength));

	m_fullscreen_blur.reset(new blur(device, m_scale->resolution(), D3DFMT_A8R8G8B8, blur_strength));

    m_scale_to_bloom_source.reset(
        new scale(device, m_blur->resolution(), scale::downscale_4, D3DFMT_A8R8G8B8));
    m_scale_to_bloom_source2.reset(
        new scale(device, m_scale_to_bloom_source->resolution(), scale::downscale_4, D3DFMT_A8R8G8B8));

    m_bloom.reset(
        new bloom(device, *bloom_parameter,
        m_scale_to_bloom_source->resolution(), D3DFMT_A8R8G8B8));

    m_noise.reset(new noise(device, *noise_parameter));
    
	m_light_scattering.reset(new light_scattering(device, *light_scattering_parameter, resolution, D3DFMT_A8R8G8B8));
    
}

void post_processing::update(float time)
{
    //if (m_adapted_luminance)
    //    m_current_adapted_luminance = m_adapted_luminance->value_at(time);

    m_bloom->update(time);
    m_bright_pass->update(time);
    //m_bright_pass2->update(time);
    m_tone_mapping->update(time);
    m_noise->update(time);

    //
    m_blur->update(time);
	m_fullscreen_blur->update(time);
    m_dof->update(time);
	m_light_scattering->update(time);
}

void post_processing::execute()
{
    set_render_state(device(), D3DRS_ALPHABLENDENABLE, FALSE);
    set_render_state(device(), D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
    set_render_state(device(), D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

    //m_scale->execute(m_render_target_texture);
    //m_bright_pass->draw(m_scale->result());
    //m_blur->draw(m_bright_pass->result());

    //m_scale_to_bloom_source->execute(m_blur->result());
    //m_bloom->draw(m_scale_to_bloom_source->result());

    //m_tone_mapping->draw(m_render_target_texture,
    //    m_bloom->result());

    //m_dof->draw(*m_render_target_texture);
    m_bright_pass->draw(*m_render_target_texture);
	//UUSI
	//m_light_scattering->draw(m_bright_pass->result());
	//m_scale->execute(m_light_scattering->isEnabled() ? m_light_scattering->result() : m_bright_pass->result());
	//UUSI END
    //ALKUP
    m_scale->execute(m_bright_pass->result());

    m_blur->draw(m_scale->result());
    m_scale_to_bloom_source->execute(m_blur->result());
	//UUSI
	//m_light_scattering->draw(m_scale_to_bloom_source->result());
	//m_bloom->draw(m_light_scattering->isEnabled() ? m_light_scattering->result() : m_scale_to_bloom_source->result());
	//UUSI END
	//ALKUP
    m_bloom->draw(m_scale_to_bloom_source->result());

    //m_scale_to_bloom_source2->execute(m_bloom->result());
    
    //Jari: uusi versio, tone mapping tulos viedään noiselle
    m_tone_mapping->draw(*m_render_target_texture,
          m_bloom->result());
	m_scale->execute(m_tone_mapping->result());
	m_fullscreen_blur->draw(m_scale->result());
	m_dof->draw(m_tone_mapping->result(), m_fullscreen_blur->result());
	//m_noise->draw(m_dof->result());
	m_light_scattering->draw(m_dof->result());
	m_noise->draw(m_light_scattering->isEnabled() ? m_light_scattering->result() : m_dof->result());

	//ALKUP
	//m_noise->draw(m_tone_mapping->result());
    
	
    
    //m_noise->draw(*m_render_target_texture);
}

void post_processing::add_adapted_luminance(float delta)
{
    m_current_adapted_luminance += delta;

    if (m_current_adapted_luminance < 0.0f)
        m_current_adapted_luminance = 0.01f;
}

std::shared_ptr<texture_2d> post_processing::input_color()
{
    return m_render_target_texture;
}

// scoped_post_processing

scoped_post_processing::scoped_post_processing(
    post_processing& post_processing)
:   m_post_processing(post_processing)
,	m_old(render_target(post_processing.device(), 0))
{
    com_ptr<IDirect3DSurface9> surface
        = surface_level(*post_processing.input_color(), 0);

    set_render_target(post_processing.device(), surface, 0);
}

scoped_post_processing::~scoped_post_processing()
{
    try
    {
        set_render_target(m_post_processing.device(), m_old, 0);
    }
    catch(...)
    {
        log("exception in ~scoped_post_processing");
        assert(false);
    }
}

// free functions

namespace {
    struct texcoord_vertex
    {
        float x, y, z;
        float u, v;
    };
}

void fill_fullscreen_quad(graphics_device& device)
{
    vector2f min(0, 0);
    vector2f max(1, 1);

    fill_quad(device, min, max);
}

void fill_quad(graphics_device& device, vector2f min, vector2f max)
{
    vector2f min_uv(0, 0);
    vector2f max_uv(1, 1);

    UINT fvf = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(1);
    dynamic_vertex_buffer buf(device, sizeof(texcoord_vertex) * 4,
        fvf, 0);

    texcoord_vertex* p = buf.data<texcoord_vertex>();

    D3DVIEWPORT9 view = viewport(device);
    
    // mapping texels to pixels by offset by 0.5
    float tmp_x = 0.5f / view.Width; //device.parameters().BackBufferWidth;
    float tmp_y = 0.5f / view.Height; //device.parameters().BackBufferHeight;
    min -= vector2f(tmp_x, tmp_y);
    max -= vector2f(tmp_x, tmp_y);

    //rotCenter -= vector2f(tmp_x, tmp_y);

    // Direct3D assumes that the viewport clipping volume ranges from
    // -1.0 to 1.0 in X, and from 1.0 to -1.0 in Y.
    min[0] = (min[0] * 2.0f) - 1.0f;
    max[0] = (max[0] * 2.0f) - 1.0f;
    min[1] = 1.0f + -min[1] * 2.0f;
    max[1] = 1.0f + -max[1] * 2.0f;

    p->x = min[0];
    p->y = min[1];
    p->z = 0.0;
    p->u = min_uv[0];
    p->v = min_uv[1];
    p++;

    p->x = max[0];
    p->y = min[1];
    p->z = 0.0f;
    p->u = max_uv[0];
    p->v = min_uv[1];
    p++;

    p->x = max[0];
    p->y = max[1];
    p->z = 0.0f;
    p->u = max_uv[0];
    p->v = max_uv[1];
    p++;

    p->x = min[0];
    p->y = max[1];
    p->z = 0.0f;
    p->u = min_uv[0];
    p->v = max_uv[1];

    buf.use(0, sizeof(texcoord_vertex));

    scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
    scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
    scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);
    scoped_render_state z_write(device, D3DRS_ZWRITEENABLE, FALSE);

    draw_primitive(device, D3DPT_TRIANGLEFAN, 0, 2);
}


void fill_quad(graphics_device& device, vector2f min, vector2f max, vector2f rotCenter, float rotation)
{
    vector2f min_uv(0, 0);
    vector2f max_uv(1, 1);

    UINT fvf = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(1);
    dynamic_vertex_buffer buf(device, sizeof(texcoord_vertex) * 4,
        fvf, 0);

    texcoord_vertex* p = buf.data<texcoord_vertex>();

    float rotSin = sin(D3DXToRadian(rotation));
    float rotCos = cos(D3DXToRadian(rotation));

    D3DVIEWPORT9 view = viewport(device);
    
    // mapping texels to pixels by offset by 0.5
    float tmp_x = 0.5f / view.Width; //device.parameters().BackBufferWidth;
    float tmp_y = 0.5f / view.Height; //device.parameters().BackBufferHeight;
    min -= vector2f(tmp_x, tmp_y);
    max -= vector2f(tmp_x, tmp_y);

    //rotCenter -= vector2f(tmp_x, tmp_y);

    // Direct3D assumes that the viewport clipping volume ranges from
    // -1.0 to 1.0 in X, and from 1.0 to -1.0 in Y.
    min[0] = (min[0] * 2.0f) - 1.0f;
    max[0] = (max[0] * 2.0f) - 1.0f;
    min[1] = 1.0f + -min[1] * 2.0f;
    max[1] = 1.0f + -max[1] * 2.0f;

    rotCenter[0] = min[0] + (max[0] - min[0])/2.0f;
    rotCenter[1] = min[1] + (max[1] - min[1])/2.0f;

    p->x = min[0];
    p->y = min[1];

    //Rotation
    p->x -= rotCenter[0];
    p->y -= rotCenter[1];
    float rotatedX = p->x * rotCos - p->y * rotSin;
    float rotatedY = p->x * rotSin + p->y * rotCos;
    p->x = rotatedX + rotCenter[0];
    p->y = rotatedY + rotCenter[1];
    
    p->z = 0.0;
    p->u = min_uv[0];
    p->v = min_uv[1];
    p++;

    p->x = max[0];
    p->y = min[1];

    //Rotation
    p->x -= rotCenter[0];
    p->y -= rotCenter[1];
    rotatedX = p->x * rotCos - p->y * rotSin;
    rotatedY = p->x * rotSin + p->y * rotCos;
    p->x = rotatedX + rotCenter[0];
    p->y = rotatedY + rotCenter[1];
    
    p->z = 0.0f;
    p->u = max_uv[0];
    p->v = min_uv[1];
    p++;

    p->x = max[0];
    p->y = max[1];

    //Rotation
    p->x -= rotCenter[0];
    p->y -= rotCenter[1];
    rotatedX = p->x * rotCos - p->y * rotSin;
    rotatedY = p->x * rotSin + p->y * rotCos;
    p->x = rotatedX + rotCenter[0];
    p->y = rotatedY + rotCenter[1];
    
    p->z = 0.0f;
    p->u = max_uv[0];
    p->v = max_uv[1];
    p++;

    p->x = min[0];
    p->y = max[1];

    //Rotation
    p->x -= rotCenter[0];
    p->y -= rotCenter[1];
    rotatedX = p->x * rotCos - p->y * rotSin;
    rotatedY = p->x * rotSin + p->y * rotCos;
    p->x = rotatedX + rotCenter[0];
    p->y = rotatedY + rotCenter[1];
    
    p->z = 0.0f;
    p->u = min_uv[0];
    p->v = max_uv[1];


    buf.use(0, sizeof(texcoord_vertex));
    /*
    D3DXVECTOR2 xRotCenter;
    xRotCenter.x = rotCenter[0];
    xRotCenter.y = rotCenter[1];

    D3DXMATRIX identity, transform2d;
    D3DXMatrixIdentity(&identity);
    D3DXMatrixIdentity(&transform2d);

    D3DXMATRIX projection;

    D3DXMatrixAffineTransformation2D(&transform2d,1.0f,&xRotCenter,45.0f,NULL);

    D3DXMatrixOrthoOffCenterLH(&projection,
        static_cast<float>(view.X),
        static_cast<float>(view.X + view.Width),
        static_cast<float>(view.Y),
        static_cast<float>(view.Y + view.Height),
        0.0f, 1.0f);
        
    projection = projection * transform2d;

    set_transform(device, D3DTS_WORLD, &identity);
    set_transform(device, D3DTS_VIEW, &identity);
    set_transform(device, D3DTS_PROJECTION, &projection);
    */
    scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
    scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
    scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);

    scoped_render_state z_write(device, D3DRS_ZWRITEENABLE, FALSE);

    //d3d9::set_vertex_shader(device, com_ptr<IDirect3DVertexShader9>());
    //d3d9::set_pixel_shader(device, com_ptr<IDirect3DPixelShader9>());

    draw_primitive(device, D3DPT_TRIANGLEFAN, 0, 2);
}

float gaussian_distribution(float x, float y, float rho)
{
    float g = 1.0f / std::sqrt( 2.0f * 3.1415f * rho * rho );
    g *= std::exp( -(x*x + y*y) / (2.0f * rho * rho));

    return g;
}

std::string const& pass_through_vertex_shader_hlsl_with_texcoord()
{
    char source[] =
        "struct input_vertex\n"
        "{\n"
        "    float4 position : POSITION;\n"
        "    float2 texcoord : TEXCOORD0;\n"
        "};\n"

        "void main_vs(inout input_vertex input)\n"
        "{\n"
        "}\n";
    static std::string hlsl(source);
    return hlsl;
}

}}} // namespace pt::d3d9::rendering
