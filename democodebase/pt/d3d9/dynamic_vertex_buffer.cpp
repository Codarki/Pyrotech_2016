#include "dynamic_vertex_buffer.h"
#include "device_convenience.h"
#include "error.h"
#include "graphics_device.h"
#include "pt/format.h"
#include "pt/log.h"

namespace pt { namespace d3d9 {

namespace {
	static std::shared_ptr<detail::static_dynamic_vertex_buffers> s_buffers;
} // anonymous namespace

namespace detail {
    class static_dynamic_vertex_buffers : device_child
    {
    public:
        static_dynamic_vertex_buffers(graphics_device& device)
        :   device_child(device)
        {
        }

        //void clear_native_buffers()
        //{
        //    for (buffer_map::iterator i = m_buffers.begin();
        //        i != m_buffers.end(); ++i)
        //    {
        //        i->second.vertex_buffer.release_native();
        //    }
        //}
        void ensure_lenght(graphics_device& device, DWORD bytes,
            DWORD fvf, DWORD lock_flags)
        {
            d3d9::dynamic_vertex_buffer buf(device, bytes, fvf, lock_flags);
        }

        dynamic_vertex_buffer::buffer* find_or_create_buffer(DWORD bytes,
            DWORD fvf)
        {
            key key;
            key.fvf = fvf;
            key.device = &device();
            buffer_map::iterator i = m_buffers.find(key);

            DWORD const buffer_size
                = i != m_buffers.end() ? i->second.vertex_buffer.bytes() : 0;

            if (i == m_buffers.end() || buffer_size < bytes)
            {
                if (i != m_buffers.end())
                {
                    log("Throwing away dynamic vertex buffer");
                    assert(!i->second.in_use);
                    assert(!i->second.locked);
                    //i->second.vertex_buffer.release_native();

                    i = m_buffers.erase(i);
                }

                log(format("Creating dynamic vertex buffer %1% bytes %2% fvf") % bytes % fvf);

                dynamic_vertex_buffer::buffer b(device(), bytes, fvf);
                b.in_use = false;
                b.locked = false;

                i = m_buffers.insert(buffer_map::value_type(key, b)).first;
            }

            assert(i != m_buffers.end());
            return &i->second;
        }

    public: // interface required by device_child
        void before_destruction()
        {
            m_buffers.clear();
        }
        void before_reset()
        {
            //clear_native_buffers();
        }
        void after_reset()
        {
        }

    private: // typedefs
        struct key
        {
            DWORD fvf;
            graphics_device* device;

            bool operator<(key const& other) const
            {
                if (fvf < other.fvf)
                    return false;
                else if (fvf > other.fvf)
                    return true;

                return device < other.device;
            }

        };
        typedef std::map<key,dynamic_vertex_buffer::buffer> buffer_map;

    private: // data members
        // FVF to vertex buffer map.
        buffer_map m_buffers;
    };
} // namespace detail

dynamic_vertex_buffer::dynamic_vertex_buffer(graphics_device& device,
    DWORD bytes, DWORD fvf, DWORD lock_flags)
:   device_reference(device)
,   m_fvf(fvf)
{
    if (!s_buffers)
    {
        s_buffers.reset(new detail::static_dynamic_vertex_buffers(device));
    }

    m_buffer = s_buffers->find_or_create_buffer(bytes, m_fvf);

    DWORD const buffer_bytes = m_buffer->vertex_buffer.bytes();

    assert(m_buffer->vertex_buffer.native());
    assert(!m_buffer->in_use);

    if (buffer_bytes - m_buffer->current_position < bytes)
    {
        lock_flags |= D3DLOCK_DISCARD;
        m_buffer->current_position = 0;
    }
    else
    {
        lock_flags |= D3DLOCK_NOOVERWRITE;
    }

    check_dx(m_buffer->vertex_buffer.native()->Lock(
        m_buffer->current_position, bytes, &m_data, lock_flags),
        "IDirect3DVertexBuffer9::Lock");

    m_start_offset = m_buffer->current_position;
    m_buffer->current_position += bytes;

    m_buffer->in_use = true;
    m_buffer->locked = true;
}

dynamic_vertex_buffer::~dynamic_vertex_buffer()
{
    unlock();
    m_buffer->in_use = false;
}

void dynamic_vertex_buffer::use(UINT stream_number, UINT stride)
{
    unlock();
    set_fvf(device(), m_fvf);
    set_stream_source(device(), stream_number,
        m_buffer->vertex_buffer.native(), m_start_offset, stride);
}

void dynamic_vertex_buffer::ensure_lenght(graphics_device& device,
    DWORD bytes, DWORD fvf)
{
    if (!s_buffers)
        s_buffers.reset(new detail::static_dynamic_vertex_buffers(device));

    s_buffers->ensure_lenght(device, bytes, fvf, 0);
}

void dynamic_vertex_buffer::clean_up()
{
    s_buffers.reset();
}

void dynamic_vertex_buffer::unlock()
{
    if (m_buffer && m_buffer->locked)
    {
        m_buffer->vertex_buffer.native()->Unlock();
        m_buffer->locked = false;
    }
}

namespace {
    vertex_buffer create(graphics_device& device, DWORD bytes, DWORD fvf)
    {
        DWORD usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
        D3DPOOL pool = D3DPOOL_DEFAULT;

        return vertex_buffer(device, bytes, usage, fvf, pool);
    }
}

dynamic_vertex_buffer::buffer::buffer(graphics_device& device, DWORD bytes,
    DWORD fvf)
:   vertex_buffer(create(device, bytes, fvf))
{
    current_position = 0;
}

}} // namespace pt::d3d9
