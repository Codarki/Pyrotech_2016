#include "spline_test.h"
#include "pt/d3d9/vertex_buffer.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/math/kb_spline.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace content {

namespace {
    static vector3f const min_limit = vector3f(0, 0, 50);
    static vector3f const max_size = vector3f(1020, 760, 200);

    typedef boost::mt19937 random_generator_type;
    typedef boost::uniform_real<float> distribution_type;
    typedef boost::variate_generator<random_generator_type&, distribution_type>
        gen_type;

    random_generator_type generator(42u);
    gen_type random_gen(generator, distribution_type(0, 1));

    static size_t const spline_keys = 10;
    static size_t const vertices_per_interval = 20;
    static size_t const vertex_count = (spline_keys - 1) * vertices_per_interval;

    DWORD fvf = D3DFVF_XYZ;
    struct vertex
    {
        float x,y,z;
    };
} // anonymous namespace

spline_test::spline_test(d3d9::graphics_device& device)
{
    // Create random spline.
    math::data_set<float> positions[3];
    std::vector<math::kochanek_bartels_parameters> parameters[3];

    float time = 0.0f;
    for (size_t i = 0; i < spline_keys; ++i)
    {
        positions[0].add(time, min_limit[0] + random_gen() * max_size[0]);
        positions[1].add(time, min_limit[1] + random_gen() * max_size[1]);
        positions[2].add(time, min_limit[2] + random_gen() * max_size[2]);
        time += 1.0f;

        parameters[0].push_back(math::kochanek_bartels_parameters(0.0f, 0, 0));
        parameters[1].push_back(math::kochanek_bartels_parameters(0.0f, 0, 0));
        parameters[2].push_back(math::kochanek_bartels_parameters(0.0f, 0, 0));
    }

    math::kb_data_set<float> spline_data[3];
    spline_data[0] = math::kb_data_set<float>(positions[0], parameters[0]);
    spline_data[1] = math::kb_data_set<float>(positions[1], parameters[1]);
    spline_data[2] = math::kb_data_set<float>(positions[2], parameters[2]);

    //math::kb_spline3f_data_set spline_data(positions);
    //spline_data.at(0).set_parameters(parameters[0]);
    //spline_data.at(1).set_parameters(parameters[1]);
    //spline_data.at(2).set_parameters(parameters[2]);
    //spline_data.calculate_tangents();
    //math::kb_spline3f spline(spline_data);
    math::kb_spline3f spline(spline_data);

    //math::catmull_rom_spline3f spline2(positions);

    // Create vertex buffer from spline.
    DWORD bytes = static_cast<DWORD>(sizeof(vertex) * vertex_count);
    DWORD usage = 0;
    D3DPOOL pool = D3DPOOL_MANAGED;
    m_vertex_buffer.reset(
        new d3d9::vertex_buffer(device, bytes, usage, fvf, pool));

    d3d9::vertex_buffer_lock lock(m_vertex_buffer->native(), 0, bytes, 0);

    float* data = lock.data<float>();

    time = 0.0f;
    float dt = 1.0f / vertices_per_interval;
    for (size_t i = 0; i < vertex_count; ++i)
    {
        vector3f result = spline.value_at(time);

        *data++ = result[0];
        *data++ = result[1];
        *data++ = result[2];

        time += dt;
    }
}

void spline_test::draw() const
{
    d3d9::graphics_device& device = m_vertex_buffer->device();

    // Bind buffer.
    UINT offset_in_bytes = 0;
    UINT stride = static_cast<UINT>(sizeof(vertex));
    set_stream_source(device, 0, m_vertex_buffer->native(), offset_in_bytes,
        stride);

    // Set transformations.
    D3DXMATRIX identity;
    D3DXMATRIX projection;
    D3DXMatrixIdentity(&identity);
    D3DVIEWPORT9 view = viewport(device);
    D3DXMatrixOrthoOffCenterLH(&projection,
        static_cast<float>(view.X),
        static_cast<float>(view.X + view.Width),
        static_cast<float>(view.Y + view.Height),
        static_cast<float>(view.Y),
        0.0f, 800.0f);

    set_transform(device, D3DTS_WORLD, &identity);
    set_transform(device, D3DTS_VIEW, &identity);
    set_transform(device, D3DTS_PROJECTION, &projection);

    // Set stuff
    vector4f color2 = vector4f(0.8f, 0.8f, 0.8f, 0.8f);
    D3DCOLOR d3d_color = D3DCOLOR_COLORVALUE(color2[0], color2[1], color2[2],
        color2[3]);

    d3d9::scoped_render_state texture_factor(device, D3DRS_TEXTUREFACTOR,
        d3d_color);
    d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_NONE);
    d3d9::scoped_render_state zenable(device, D3DRS_ZENABLE, FALSE);
    d3d9::scoped_render_state lightning(device, D3DRS_LIGHTING, FALSE);
    d3d9::scoped_render_state alpha_blend(device, D3DRS_ALPHABLENDENABLE, FALSE);

    d3d9::scoped_texture_stage_state color_op(device, 0, D3DTSS_COLOROP,
        D3DTOP_SELECTARG1);
    d3d9::scoped_texture_stage_state color_arg1(device, 0, D3DTSS_COLORARG1,
        D3DTA_TFACTOR);

    d3d9::set_fvf(device, fvf);

    d3d9::set_pixel_shader(device, com_ptr<IDirect3DPixelShader9>());
    d3d9::set_vertex_shader(device, com_ptr<IDirect3DVertexShader9>());

    d3d9::draw_primitive(device, D3DPT_LINESTRIP, 0, vertex_count - 1);

    d3d9::set_fvf(device, 0);
}

void spline_test::update(float)
{
}

} // namespace content
