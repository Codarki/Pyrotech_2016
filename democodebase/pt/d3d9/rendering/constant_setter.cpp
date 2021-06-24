#include "constant_setter.h"
#include <cassert>

namespace pt { namespace d3d9 { namespace rendering {

constant_setter::constant_setter()
{
}

void constant_setter::associate_to(constant_buffer const& buffer,
    d3d9::constant const& constant)
{
    m_constant = &constant;
    m_buffer = &buffer;
}

d3d9::constant const& constant_setter::constant() const
{
    assert(m_constant);
    return *m_constant;
}

constant_buffer const& constant_setter::constants() const
{
    assert(m_buffer);
    return *m_buffer;
}

}}} // namespace pt::d3d9::rendering
