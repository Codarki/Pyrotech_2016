#pragma once
#ifndef PT_BASIC_TYPES_H
#define PT_BASIC_TYPES_H

#ifdef PT_USE_BOOST_BASIC_TYPES

//#include <cstdio>
//#include <boost/cstdlib.hpp>
#include <boost/cstdint.hpp>
//#include <boost/assert.hpp>
//#include <boost/cast.hpp>

namespace pt {

// Platform independent size types. Useful for serialization.
typedef boost::uint64_t uint64; //!< Platform independent uint64 type
typedef boost::uint32_t uint32; //!< Platform independent uint32 type
typedef boost::uint16_t uint16; //!< Platform independent uint16 type
typedef boost::uint8_t  uint8; //!< Platform independent int8 type
typedef boost::int64_t int64; //!< Platform independent int64 type
typedef boost::int32_t int32; //!< Platform independent int32 type
typedef boost::int16_t int16; //!< Platform independent int16 type
typedef boost::int8_t  int8; //!< Platform independent int8 type

typedef float  float32; //!< Platform independent float32 type
typedef double float64; //!< Platform independent float64 type

} // namespace pt

#else // !PT_USE_BOOST_BASIC_TYPES

#include <cstddef>

namespace pt {

typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef signed __int64 int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;

typedef float float32;
typedef double float64;

} // namespace pt

#endif // !PT_USE_BOOST_BASIC_TYPES

#endif
