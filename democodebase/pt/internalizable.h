#ifndef PT_INTERNALIZABLE_H
#define PT_INTERNALIZABLE_H

namespace pt {
class input_stream;

/// Interface for classes that can be internalized.
/// \par See also:
/// pt::input_stream
class internalizable
{
public:
    /// Internalize this class from \a stream.
    virtual void internalize(input_stream& stream) = 0;
    virtual ~internalizable() {}
};

} // namespace pt

#endif
