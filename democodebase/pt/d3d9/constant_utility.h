#pragma once
#ifndef PT_D3D9_CONSTANT_UTILITY_H
#define PT_D3D9_CONSTANT_UTILITY_H

#include "native.h"
#include "constant_buffer_fwd.h"
#include "pt/vector_fwd.h"
#include "pt/matrix_fwd.h"
#include "pt/std/vector.h"
#include "pt/std/cassert.h"

// .inl
#include "constant.h"
//#include "constant_buffer.h"

namespace pt { namespace d3d9 {

class constant;

// Convenience functions.

/// Sets scalar shader constant of type T.
template<typename T>
void set_constant(constant const& constant, constant_buffer const& buffer,
    T const& value);

/// Sets vector shader constant of type T.
template<typename T, int dim>
void set_constant(constant const& constant, constant_buffer const& buffer,
    vector<T,dim> const& value);

/// Sets matrix shader constant of type T.
template<typename T, int rows, int columns>
void set_constant(constant const& constant, constant_buffer const& buffer,
    matrix<T,rows,columns> const& value);

/// Sets array of vector shader constants of type T.
template<typename T, int dim>
void set_constant(constant const& constant, constant_buffer const& buffer,
    std::vector<vector<T,dim> > const& values);

/// Sets array of shader constants of type T.
template<typename T>
void set_constant(constant const& constant, constant_buffer const& buffer,
    T const values[], size_t count);

}} // namespace pt::d3d9

#include "constant_utility.inl"

#endif
