#include "device_child.h"
#include "graphics_device.h"
#include "pt/debug_only.h"
#include "pt/log.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 {

graphics_device const& device_child::device() const
{
    assert(!m_device_destructed);
    return m_device;
}
graphics_device& device_child::device()
{
    assert(!m_device_destructed);
    return m_device;
}
graphics_device& device_child::mutable_device() const
{
    assert(!m_device_destructed);
    return m_device;
}

device_child::device_child(graphics_device& device)
:   m_device(device)
,   m_device_destructed(false)
{
    m_device.add_child(this);
}

device_child::device_child(device_child const& other)
:   m_device(other.m_device)
,   m_device_destructed(other.m_device_destructed)
{
    m_device.add_child(this);
}

device_child::~device_child()
{
    try
    {
        if (!m_device_destructed)
            m_device.remove_child(this);
    }
    catch(...)
    {
        log("exception in d3d9::device_child destructor");
        assert(false);
    }
}

device_child const& device_child::operator=(
    device_child const& PT_DEBUG_ONLY(x))
{
    assert(&m_device == &x.device());
    return *this;
}

void device_child::device_destructed()
{
    m_device_destructed = true;
}

// device_reference

graphics_device const& device_reference::device() const
{
    return *m_device;
}

graphics_device& device_reference::device()
{
    return *m_device;
}

graphics_device& device_reference::mutable_device() const
{
    return *m_device;
}

device_reference::device_reference(graphics_device& device)
:   m_device(&device)
{
}

device_reference::~device_reference()
{
}

// optional_device_reference

graphics_device const& optional_device_reference::device() const
{
    return *m_device;
}

graphics_device& optional_device_reference::device()
{
    return *m_device;
}

graphics_device& optional_device_reference::mutable_device() const
{
    return *m_device;
}

bool optional_device_reference::has_device() const
{
    return m_device != 0;
}

optional_device_reference::optional_device_reference(graphics_device* device)
:   m_device(device)
{
}

optional_device_reference::~optional_device_reference()
{
}

}} // namespace pt::d3d9
