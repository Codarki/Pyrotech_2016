#pragma once
#ifndef PT_D3D9_STREAM_SOURCE_H
#define PT_D3D9_STREAM_SOURCE_H

namespace pt { namespace d3d9 {

class vertex_stream_declaration;

/// Abstract base class for all vertex stream sources.
/// See IDirect3DDevice9::SetStreamSource for more documentation.
class stream_source
{
public:
    virtual ~stream_source();

public: // interface which subclasses must implement
    virtual void use(int stream_number) const = 0;
    virtual vertex_stream_declaration const& declaration() const = 0;
    virtual size_t vertex_count() const = 0;
};

}} // namespace pt::d3d9

#endif
