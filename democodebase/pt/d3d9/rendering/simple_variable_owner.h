#pragma once
#ifndef PT_D3D9_RENDERING_SIMPLE_VARIABLE_OWNER_H
#define PT_D3D9_RENDERING_SIMPLE_VARIABLE_OWNER_H

#include "variable_owner.h"
#include "detail/setter_container.h"

namespace pt { namespace d3d9 {
class constant;
}}

namespace pt { namespace d3d9 { namespace rendering {

class constant_setter;

/// Composite of variable_setters.
/// \todo composite_variable_setter might be better name.
class simple_variable_owner : public variable_owner
{
public:
    simple_variable_owner();
    ~simple_variable_owner();

public: // interface required by variable_owner
    bool try_create_constants(constant_buffer const& constants);
    void update_target(float time, constant_buffer const& buffer) const;

protected: // interface which subclasses must implement
    /// Any setter returned from this function will get added into this
    /// composite.
    virtual std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const = 0;

protected: // hook functions
    /// Any setter returned from this function will get added into this
    /// composite.
    virtual std::unique_ptr<variable_setter> on_create_setter(
        constant_buffer const& constants, size_t index) const;

private: // noncopyable
    simple_variable_owner(simple_variable_owner const&);
    simple_variable_owner const& operator=(simple_variable_owner const&);

private: // data members
    detail::setter_container m_setters;
};

}}} // namespace pt::d3d9::rendering

#endif
