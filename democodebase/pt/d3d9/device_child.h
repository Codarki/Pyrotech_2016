#pragma once
#ifndef PT_D3D9_DEVICE_CHILD_H
#define PT_D3D9_DEVICE_CHILD_H

#include "graphics_device_fwd.h"

namespace pt { namespace d3d9 {

class device_child
{
public:
    graphics_device const& device() const;
    graphics_device& device();
    graphics_device& mutable_device() const;

protected:
    device_child(graphics_device& device);
    device_child(device_child const& other);
    virtual ~device_child();

    virtual device_child const& operator=(device_child const& x);

protected: // interface which subclasses must implement
    friend graphics_device;
    // Informs this child that its reference to the device has become
    // invalid. This happens if child outlives the device.
    void device_destructed();

    // Informs this child that device is about to be destroyed.
    virtual void before_destruction() = 0;

    // Informs this child that device is about to be reset.
    virtual void before_reset() = 0;

    // Informs this child that device is reseted.
    virtual void after_reset() = 0;

private:
    graphics_device& m_device;
    bool m_device_destructed;
};

/// Base class for classes which need a reference to graphics_device.
class device_reference
{
public:
    graphics_device const& device() const;
    graphics_device& device();
    graphics_device& mutable_device() const;

protected:
    device_reference(graphics_device& device);
    ~device_reference();

private:
    graphics_device* m_device;
};

/// Base class for classes which can have a reference to graphics_device.
class optional_device_reference
{
public:
    graphics_device const& device() const;
    graphics_device& device();
    graphics_device& mutable_device() const;
    bool has_device() const;

protected:
    optional_device_reference(graphics_device* device);
    ~optional_device_reference();

private:
    graphics_device* m_device;
};

}} // namespace pt::d3d9

#endif
