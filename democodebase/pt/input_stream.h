#pragma once
#ifndef PT_INPUT_STREAM_H
#define PT_INPUT_STREAM_H

#include "basic_types.h"
//#include "std/string.h"

namespace pt {

class string_wrapper;

/// Abstract base class for all input streams.
class input_stream
{
public:
    /// Reads \a bytes from this stream into \a data.
    virtual void read(void* data, size_t bytes) = 0;
    virtual ~input_stream() {}
};

/// Read from \a stream into a \a value. \relatesalso input_stream
//template<typename T> void read(input_stream& stream, T& value);

void read(input_stream& stream, uint8& value);
void read(input_stream& stream, int8& value);
void read(input_stream& stream, uint16& value);
void read(input_stream& stream, int16& value);
void read(input_stream& stream, pt::uint32& value);
void read(input_stream& stream, int32& value);
void read(input_stream& stream, uint64& value);
void read(input_stream& stream, int64& value);

void read(input_stream& stream, float32& value);
void read(input_stream& stream, float64& value);

/// Read null terminated ascii string from \a stream into \a str.
/// \relatesalso input_stream
void read(input_stream& stream, string_wrapper& str);
string_wrapper read_string(input_stream& stream);

/// Read a boolean value from \a stream into \a value.
/// \relatesalso input_stream
void read(input_stream& stream, bool& value);

//////
void read_array(input_stream& stream, size_t count, uint8* destination);
void read_array(input_stream& stream, size_t count, int8* destination);
void read_array(input_stream& stream, size_t count, uint16* destination);
void read_array(input_stream& stream, size_t count, int16* destination);
void read_array(input_stream& stream, size_t count, pt::uint32* destination);
void read_array(input_stream& stream, size_t count, int32* destination);
void read_array(input_stream& stream, size_t count, uint64* destination);
void read_array(input_stream& stream, size_t count, int64* destination);
void read_array(input_stream& stream, size_t count, float32* destination);
void read_array(input_stream& stream, size_t count, float64* destination);

/// \return Instance of type T read from a \a stream. \relatesalso input_stream
template<typename T>
T read(input_stream& stream);

} // namespace pt

#include "input_stream.inl"

#endif
