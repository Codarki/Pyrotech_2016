#ifndef PT_D3D9_CONSTANT_BUFFER_H
#define PT_D3D9_CONSTANT_BUFFER_H

#include "constant.h"
#include "device_child.h"

namespace pt { namespace d3d9 {

class constant_buffer : public device_reference, public constant_structure
{
public:
    constant_buffer(graphics_device& device,
        com_ptr<ID3DXConstantTable> table);
    ~constant_buffer();

    com_ptr<ID3DXConstantTable>& native() const;
    void set_defaults();

private: // data members
    mutable com_ptr<ID3DXConstantTable> m_table;
};

class constant_builder
{
public:
    constant_builder(com_ptr<ID3DXConstantTable> table, D3DXHANDLE parent,
        UINT index);
    d3d9::constant const& constant() const;

private: // data members
    d3d9::constant m_constant;
};

class constant_element_builder
{
public:
    constant_element_builder(com_ptr<ID3DXConstantTable> table,
        D3DXHANDLE array, UINT index);
    d3d9::constant const& constant() const;

private: // data members
    d3d9::constant m_constant;
};

// Free functions.
void build_constant_structure(constant_structure& structure,
    com_ptr<ID3DXConstantTable> table, D3DXHANDLE parent, UINT member_count);

//void set_constant(constant_buffer const& buffer, D3DXHANDLE h, float value);
//void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
//    vector2f const& value);
//void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
//    vector4f const& value);
//void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
//    matrix4x4f const& value);
//void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
//    std::vector<vector4f> const& value);

}} // namespace pt::d3d9

#endif
