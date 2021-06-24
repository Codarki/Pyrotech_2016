#ifndef PT_OUTPUT_STREAM_H
#define PT_OUTPUT_STREAM_H

#include "basic_types.h"
#include "std/string.h"

//#pragma warning(disable: 4344)  // behavior change: use of explicit template arguments results in call to

namespace pt {

/// Abstract base class for all output streams.
class output_stream
{
public:
    /// Writes \a bytes from \a data into this stream.
    virtual void write(void const* data, size_t bytes) = 0;
    virtual ~output_stream() {}
};

/// Writes \a value into \a stream. \relatesalso output_stream
//template<typename T>
//void write(output_stream& stream, T const& value);

void write(output_stream& stream, uint8 const& value);
void write(output_stream& stream, int8 const& value);
void write(output_stream& stream, uint16 const& value);
void write(output_stream& stream, int16 const& value);
void write(output_stream& stream, uint32 const& value);
void write(output_stream& stream, int32 const& value);
void write(output_stream& stream, uint64 const& value);
void write(output_stream& stream, int64 const& value);

void write(output_stream& stream, float32 const& value);
void write(output_stream& stream, float64 const& value);

/// Write null terminated ascii string \a str into \a stream.
/// \relatesalso output_stream
void write(output_stream& stream, std::string const& str);

/// Write a boolean \a value into \a stream. \relatesalso output_stream
void write(output_stream& stream, bool const& value);

/// Write null terminated ascii string \a str into \a stream.
/// \relatesalso output_stream
void write(output_stream& stream, char const* str);

} // namespace pt

#include "output_stream.inl"

#endif
