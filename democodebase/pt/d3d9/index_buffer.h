#ifndef PT_D3D9_INDEX_BUFFER_H
#define PT_D3D9_INDEX_BUFFER_H

#include "resource.h"
#include "native.h"
#include "pt/basic_types.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {

class index_buffer : public resource
{
public:
    index_buffer(graphics_device& device, UINT bytes, DWORD usage,
        D3DFORMAT format, D3DPOOL pool);

    com_ptr<IDirect3DIndexBuffer9>& native();
    com_ptr<IDirect3DIndexBuffer9> const& native() const;

    void use() const;
    size_t count() const;

protected: // interface which subclasses must implement
    void before_destruction();
    void before_reset();
    void after_reset();

private: // data members
    size_t m_count;
    com_ptr<IDirect3DIndexBuffer9> m_native;
};

class scoped_index_buffer_lock
{
public:
    scoped_index_buffer_lock(index_buffer& buffer);
    ~scoped_index_buffer_lock();

    uint16* data();

private: // noncopyable
    scoped_index_buffer_lock(scoped_index_buffer_lock const&);
    scoped_index_buffer_lock const& operator=(scoped_index_buffer_lock const&);

private:
    index_buffer& m_buffer;
    void* m_data;
};

index_buffer create_default_16bit_index_buffer(graphics_device& device,
    UINT bytes);
std::unique_ptr<index_buffer> create_default_16bit_unique_index_buffer(graphics_device& device,
    UINT bytes);

}} // namespace pt::d3d9

#endif
