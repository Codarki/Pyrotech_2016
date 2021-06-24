#include "constant_buffer.h"
#include "error.h"
#include "graphics_device.h"
#include "pt/vector.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 {

// constant_buffer

constant_buffer::constant_buffer(graphics_device& device,
    com_ptr<ID3DXConstantTable> table)
:   device_reference(device)
,   m_table(table)
{
    D3DXCONSTANTTABLE_DESC table_desc;
    check_dx(m_table->GetDesc(&table_desc), "ID3DXConstantTable::GetDesc");

    build_constant_structure(*this, m_table, 0, table_desc.Constants);
}

constant_buffer::~constant_buffer()
{
}

com_ptr<ID3DXConstantTable>& constant_buffer::native() const
{
    return m_table;
}

void constant_buffer::set_defaults()
{
    check_dx(m_table->SetDefaults(device().native().get()),
        "ID3DXConstantTable::SetDefaults");
}

// constant_builder

constant_builder::constant_builder(com_ptr<ID3DXConstantTable> table,
    D3DXHANDLE parent, UINT index)
{
    D3DXHANDLE handle = table->GetConstant(parent, index);

    D3DXCONSTANT_DESC desc[100];
    UINT count = 100;
    check_dx(table->GetConstantDesc(handle, desc, &count),
        "ID3DXConstantTable::GetConstantDesc");

    assert(count == 1);

    m_constant.populate(handle, desc[0], table);
}

d3d9::constant const& constant_builder::constant() const
{
    return m_constant;
}

constant_element_builder::constant_element_builder(
    com_ptr<ID3DXConstantTable> table, D3DXHANDLE array, UINT index)
{
    D3DXHANDLE handle = table->GetConstantElement(array, index);

    D3DXCONSTANT_DESC desc[100];
    UINT count = 100;
    check_dx(table->GetConstantDesc(handle, desc, &count),
        "ID3DXConstantTable::GetConstantDesc");

    assert(count == 1);

    m_constant.populate(handle, desc[0], table);
}

d3d9::constant const& constant_element_builder::constant() const
{
    return m_constant;
}

// free functions

void build_constant_structure(constant_structure& structure,
    com_ptr<ID3DXConstantTable> table, D3DXHANDLE parent, UINT member_count)
{
    for (UINT i = 0; i < member_count; ++i)
    {
        structure.push_back(constant_builder(table, parent, i).constant());
    }
}

void set_constant(constant_buffer const& buffer, D3DXHANDLE h, float value)
{
    check_dx(buffer.native()->SetFloat(
        buffer.device().native().get(), h, value),
        "ID3DXConstantTable::SetFloat");
}

void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
    vector2f const& value)
{
    check_dx(buffer.native()->SetFloatArray(
        buffer.device().native().get(), h, &value[0], 2),
        "ID3DXConstantTable::SetFloatArray");
}

void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
    vector4f const& value)
{
    D3DXVECTOR4 vector;
    vector.x = value[0];
    vector.y = value[1];
    vector.z = value[2];
    vector.w = value[3];

    check_dx(buffer.native()->SetVector(
        buffer.device().native().get(), h, &vector),
        "ID3DXConstantTable::SetVector");
}

void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
    matrix4x4f const& value)
{
    D3DXMATRIX matrix(&value(0, 0));

    check_dx(buffer.native()->SetMatrix(
        buffer.device().native().get(), h, &matrix),
        "ID3DXConstantTable::SetMatrix");
}

void set_constant(constant_buffer const& buffer, D3DXHANDLE h,
    std::vector<vector4f> const& value)
{
    D3DXVECTOR4 const* src = reinterpret_cast<D3DXVECTOR4 const*>(&value[0][0]);

    check_dx(buffer.native()->SetVectorArray(
        buffer.device().native().get(), h, src, static_cast<UINT>(value.size())),
        "ID3DXConstantTable::SetVectorArray");
}

}} // namespace pt::d3d9
