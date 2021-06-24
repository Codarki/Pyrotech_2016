#include "parameter_node_setter_create.h"
#include "parameter_node_setter.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/any_error.h"
#include "pt/vector.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    std::unique_ptr<constant_setter> create_scalar_parameter_node_setter(
        std::shared_ptr<parameter_node> node, constant const& c)
    {
        D3DXPARAMETER_TYPE param_type= c.desc().Type;

        switch (param_type)
        {
        case D3DXPT_FLOAT:
            return std::unique_ptr<constant_setter>(new parameter_node_setter<float>(node));
        };
        throw any_error("not implemented");
    }

    template<typename T>
    std::unique_ptr<constant_setter> create_vector_parameter_node_setter(
        std::shared_ptr<parameter_node> node, constant const& c)
    {
        size_t count = c.desc().Columns;
        switch (count)
        {
        //case 1:
        //    return make_shared_ptr(
        //        new parameter_node_setter<vector<T,1> >(node));
        //case 2:
        //    return make_shared_ptr(
        //        new parameter_node_setter<vector<T,2> >(node));
        case 3:
            return std::unique_ptr<constant_setter>(
                new parameter_node_setter<vector<T,3> >(node));
        case 4:
            return std::unique_ptr<constant_setter>(
                new parameter_node_setter<vector<T,4> >(node));
        };
        throw any_error("not implemented");
    }

    std::unique_ptr<constant_setter> create_vector_parameter_node_setter(
        std::shared_ptr<parameter_node> node, constant const& c)
    {
        D3DXPARAMETER_TYPE param_type = c.desc().Type;

        switch (param_type)
        {
        case D3DXPT_FLOAT:
            return create_vector_parameter_node_setter<float>(node, c);

        //case D3DXPT_INT:
        //    return create_vector_parameter_node_setter<int>(node, c);

        default:
            throw any_error("not implemented");
        };
    }
} // anonymous namespace

std::unique_ptr<constant_setter> create_parameter_node_setter(
    std::shared_ptr<parameter_node> node, constant const& c)
{
    D3DXPARAMETER_CLASS param_class = c.desc().Class;

    switch(param_class)
    {
    case D3DXPC_SCALAR:
        return create_scalar_parameter_node_setter(node, c);

    case D3DXPC_VECTOR:
        return create_vector_parameter_node_setter(node, c);
        break;

    case D3DXPC_MATRIX_COLUMNS:
        break;
    case D3DXPC_MATRIX_ROWS:
    case D3DXPC_OBJECT:
    case D3DXPC_STRUCT:
    default:
        throw any_error("not implemented");
    };
    throw any_error("not implemented");
}

}}} // namespace pt::d3d9::rendering
