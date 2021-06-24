#include "constant_buffer.h"
#include "pt/any_error.h"
#include "pt/string_wrapper.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 {

// constant_structure

size_t constant_structure::count() const
{
    return m_constants.size();
}

constant const& constant_structure::at(size_t index) const
{
    assert(index < count());
    return m_constants[index];
}

constant const& constant_structure::by_name(std::string const& name) const
{
    if (constant const* c = try_get(name))
        return *c;

    throw any_error("Constant " + name + " not found");
}

constant const* constant_structure::try_get(std::string const& name) const
{
    for (std::vector<constant>::const_iterator i = m_constants.begin();
        i != m_constants.end(); ++i)
    {
        if (i->name() == name)
            return &(*i);
    }
    return 0;
}

constant_structure::~constant_structure()
{
}

void constant_structure::push_back(constant const& constant)
{
    m_constants.push_back(constant);
}

// constant

constant::constant()
:   m_handle(0)
{
}

void constant::populate(D3DXHANDLE handle, D3DXCONSTANT_DESC const& desc,
    com_ptr<ID3DXConstantTable> table)
{
    m_handle = handle;
    m_desc = desc;

    // Structure constant has always m_elements size of 1.
    // Constant arrays has always m_elements size of >1.
    // All other constants have m_elements size of 0.

    if (m_desc.Elements == 1)
    {
        if (m_desc.StructMembers > 0)
        {
            // This constant is a structure of constants.
            assert(m_desc.Class == D3DXPC_STRUCT);

            constant_structure s;
            build_constant_structure(s, table, m_handle, m_desc.StructMembers);
            m_elements.push_back(s);
        }
    }
    else
    {
        // This constant is an array of constants.
        for (UINT i = 0; i < m_desc.Elements; ++i)
        {
            constant_structure s;
            s.push_back(constant_element_builder(table, m_handle, i).constant());
            m_elements.push_back(s);
        }
    }
}

D3DXCONSTANT_DESC const& constant::desc() const
{
    return m_desc;
}

std::string constant::name() const
{
    return m_desc.Name;
}

D3DXHANDLE constant::handle() const
{
    assert(m_handle);
    return m_handle;
}

size_t constant::element_count() const
{
    return m_desc.Elements;
}

bool constant::is_structure() const
{
    return m_elements.size() == 1;
}

bool constant::is_array() const
{
    return m_elements.size() > 1;
}

constant_structure const& constant::at(size_t index) const
{
    assert(index < m_elements.size());
    return m_elements[index];
}

constant_structure const& constant::operator[](size_t index) const
{
    assert(index < m_elements.size());
    return m_elements[index];
}

constant const& constant::by_name(std::string const& name) const
{
    assert(is_structure());
    return m_elements[0].by_name(name);
}

}} // namespace pt::d3d9
