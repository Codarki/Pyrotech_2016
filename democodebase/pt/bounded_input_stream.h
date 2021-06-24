#pragma once
#ifndef PT_BOUNDED_INPUTSTREAM_H
#define PT_BOUNDED_INPUTSTREAM_H

#include "input_stream.h"

namespace pt {

/// Input stream with bounds
class bounded_input_stream : public input_stream
{
public:
    /// Seek to a position \a offset in bytes in this stream.
    virtual void seek(size_t offset) = 0;

    /// \return Current position of this stream.
    virtual size_t position() const = 0;

    /// \return Current position of this stream.
    virtual size_t size() const = 0;
};

} // namespace pt

#endif
