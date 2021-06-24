#ifndef PT_EXTERNALIZABLE_H
#define PT_EXTERNALIZABLE_H

namespace pt {
class output_stream;

/// Interface for classes that can be externalized.
/// \par See also:
/// pt::output_stream
class externalizable
{
public:
    /// Externalize this class into \a stream.
    virtual void externalize(output_stream const& stream) = 0;
    virtual ~externalizable() {}
};

} // namespace pt

#endif
