#include "constant_utility.h"
#include "constant_buffer.h"
#include "graphics_device.h"
#include "error.h"
#include "utility.h"
#include "pt/any_error.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 {

namespace detail {
    void check_is_scalar(constant const& constant)
    {
        if (constant.desc().Class != D3DXPC_SCALAR)
            throw any_error("constant is not scalar");
    }
    void check_is_vector(constant const& constant)
    {
        if (constant.desc().Class != D3DXPC_VECTOR)
            throw any_error("constant is not vector");
    }
    void check_is_matrix(constant const& constant)
    {
        if (constant.desc().Class != D3DXPC_MATRIX_COLUMNS
            && constant.desc().Class != D3DXPC_MATRIX_ROWS)
        {
            throw any_error("constant is not matrix");
        }
    }

    void check_component_type(constant const& constant, float)
    {
        if (constant.desc().Type != D3DXPT_FLOAT)
            throw any_error("constant is not float");
    }
    void check_component_type(constant const& constant, int)
    {
        if (constant.desc().Type != D3DXPT_INT)
            throw any_error("constant is not int");
    }
    void check_component_type(constant const& constant, bool)
    {
        if (constant.desc().Type != D3DXPT_BOOL)
            throw any_error("constant is not bool");
    }

    void set_constant(constant_buffer const& buffer,
        constant const& constant, float value)
    {
        check_dx(buffer.native()->SetFloat(
            buffer.device().native().get(), constant.handle(), value),
            "ID3DXConstantTable::SetFloat");
    }

    void set_constant(constant_buffer const& buffer,
        constant const& constant, bool value)
    {
        check_dx(buffer.native()->SetBool(
            buffer.device().native().get(), constant.handle(), value),
            "ID3DXConstantTable::SetBool");
    }

    void set_vector_constant(constant_buffer const& buffer,
        constant const& constant, vector4f const& value)
    {
        D3DXVECTOR4 vector;
        vector.x = value[0];
        vector.y = value[1];
        vector.z = value[2];
        vector.w = value[3];

        check_dx(buffer.native()->SetVector(
            buffer.device().native().get(), constant.handle(), &vector),
            "ID3DXConstantTable::SetVector");
    }

    void set_matrix_constant(constant_buffer const& buffer,
        constant const& constant, matrix4x4f const& value)
    {
        //D3DXMATRIX matrix(&value(0, 0));

        D3DXMATRIX matrix = d3d9::convert(transpose(value));
        //D3DXMATRIX matrix = d3d9::convert((value));

        check_dx(buffer.native()->SetMatrix(
            buffer.device().native().get(), constant.handle(), &matrix),
            "ID3DXConstantTable::SetMatrix");
    }

    void set_vector_array_constant(constant_buffer const& buffer,
        constant const& constant, std::vector<vector4f> const& value)
    {
        D3DXVECTOR4 const* src = reinterpret_cast<D3DXVECTOR4 const*>(&value[0][0]);

        check_dx(buffer.native()->SetVectorArray(
            buffer.device().native().get(), constant.handle(), src, static_cast<UINT>(value.size())),
            "ID3DXConstantTable::SetVectorArray");
    }

    void set_matrix_array_constant(constant_buffer const& buffer,
        constant const& constant, matrix4x4f const values[], size_t count)
    {
        D3DXMATRIX sources[256];
        for (size_t i = 0; i < count; ++i)
            sources[i] = d3d9::convert(transpose(values[i]));

        check_dx(buffer.native()->SetMatrixArray(
            buffer.device().native().get(), constant.handle(), sources,
            static_cast<UINT>(count)), "ID3DXConstantTable::SetMatrixArray");
    }

} // namespace detail

}} // namespace pt::d3d9
