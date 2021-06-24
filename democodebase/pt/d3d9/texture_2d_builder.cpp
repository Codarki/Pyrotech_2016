#include "texture_2d_builder.h"
#include "texture_2d.h"
#include "enum_utility.h"
#include "error.h"
#include "graphics_device.h"
#include "device_convenience.h"
#include "pt/file_input_stream.h"
#include "pt/data_input_stream.h"
#include "pt/data_stream_content.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/exceptions/file_error.h"

namespace pt { namespace d3d9 {

namespace {
    com_ptr<IDirect3DTexture9> create_native_texture_2d(
        graphics_device& device,
        texture_2d_builder::parameters const& parameters)
    {
        return create_texture(
            device,
            parameters.width,
            parameters.height,
            parameters.levels,
            parameters.usage,
            parameters.format,
            parameters.pool);
    }

} // anonymous namespace

// texture_2d_builder

texture_2d_builder::parameters::parameters()
: width(0)
, height(0)
, levels(0)
, usage(0)
, format(D3DFMT_UNKNOWN)
, pool(D3DPOOL_DEFAULT)
{
}

texture_2d_builder::texture_2d_builder(graphics_device& device)
: m_device(&device)
{
}

texture_2d_builder& texture_2d_builder::width(UINT width)
{
    m_parameters.width = width;
    return *this;
}

texture_2d_builder& texture_2d_builder::height(UINT height)
{
    m_parameters.height = height;
    return *this;
}

texture_2d_builder& texture_2d_builder::resolution(UINT width, UINT height)
{
    m_parameters.width = width;
    m_parameters.height = height;
    return *this;
}

texture_2d_builder& texture_2d_builder::resolution(vector2i resolution)
{
    m_parameters.width = resolution[0];
    m_parameters.height = resolution[1];
    return *this;
}

texture_2d_builder& texture_2d_builder::levels(UINT levels)
{
    m_parameters.levels = levels;
    return *this;
}

texture_2d_builder& texture_2d_builder::usage(DWORD usage)
{
    m_parameters.usage = usage;
    return *this;
}

texture_2d_builder& texture_2d_builder::format(D3DFORMAT format)
{
    m_parameters.format = format;
    return *this;
}

texture_2d_builder& texture_2d_builder::pool(D3DPOOL pool)
{
    m_parameters.pool = pool;
    return *this;
}

texture_2d texture_2d_builder::build()
{
    return texture_2d(*m_device,
        create_native_texture_2d(*m_device, m_parameters));
}

texture_2d do_build(pt::path const& path, D3DPOOL pool, graphics_device& device)
{
    file_input_stream file(path);
    data_stream_content data_stream(file);
    //filesystem::path p = filesystem::complete(path);
    //filesystem::path p = filesystem::system_complete(path);
    //if (!filesystem::exists(p))
    //    throw exceptions::file_error(p, std::string("file not found"));

    UINT mip_levels = D3DX_DEFAULT;
    DWORD usage = 0;
    D3DXIMAGE_INFO src_info;
    DWORD filter = D3DX_DEFAULT;
    DWORD mip_filter = D3DX_DEFAULT;
    D3DCOLOR color_key = 0;

    //D3DFORMAT format = D3DFMT_DXT3; //D3DFMT_UNKNOWN; // D3DFMT_A8R8G8B8
    D3DFORMAT format = D3DFMT_A8R8G8B8;

	if (path.string().find("vignette") != std::string::npos)
	{
		format = D3DFMT_UNKNOWN;
	}

    com_ptr<IDirect3DTexture9> native;

    check_dx(D3DXCreateTextureFromFileInMemoryEx(
        device.native().get(),
        data_stream.data(),
        static_cast<UINT>(data_stream.size()),
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        mip_levels,
        usage,
        format,
        pool, //D3DPOOL_MANAGED,
        filter,
        mip_filter,
        color_key,
        &src_info,
        0,
        native.receive()
        ),
        (std::string("D3DXCreateTextureFromFileEx: ")+path.string()).c_str());

    size_t bytes = src_info.Width * src_info.Height
        * format_size_in_bits(src_info.Format) / 8;

    log(pt::format("Allocated %1%x%2% texture2d \"%3%\": approximately %4%")
        % src_info.Width % src_info.Height % path.string()
        % bytes_in_formatted_text(bytes));

    return texture_2d(device, native);
}

texture_2d texture_2d_builder::build(pt::path const& path)
{
    D3DPOOL pool = D3DPOOL_MANAGED;
    return do_build(path, pool, *m_device);
}

texture_2d texture_2d_builder::build_immutable(pt::path const& path)
{
    D3DPOOL pool = D3DPOOL_DEFAULT;
    return do_build(path, pool, *m_device);
}

std::shared_ptr<texture_2d> texture_2d_builder::build_shared()
{
    return std::make_shared<texture_2d>(
        *m_device, create_native_texture_2d(*m_device, m_parameters));
}

std::shared_ptr<texture_2d> texture_2d_builder::build_shared(pt::path const& path)
{
    return std::make_shared<texture_2d>(build(path));
}

std::shared_ptr<texture_2d> texture_2d_builder::build_immutable_shared(pt::path const& path)
{
    return std::make_shared<texture_2d>(build_immutable(path));
}

std::unique_ptr<d3d9::texture_2d> texture_2d_builder::build_unique_ptr()
{
    return std::unique_ptr<d3d9::texture_2d>(new texture_2d(
        *m_device, create_native_texture_2d(*m_device, m_parameters)));
}

// free functions

std::string bytes_in_formatted_text(size_t bytes)
{
    float mega = 1024.0f * 1024.0f;
    float kilo = 1024.0f;
    if (bytes >= mega)
        return (pt::format("%1%Mb") % (bytes / mega)).move_string();
    if (bytes >= kilo)
        return (pt::format("%1%Kb") % (bytes / kilo)).move_string();

    return (pt::format("%1%b") % bytes).move_string();
}

}} // namespace pt::d3d9
