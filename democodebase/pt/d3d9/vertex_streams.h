#pragma once
#ifndef PT_D3D9_VERTEX_STREAMS_H
#define PT_D3D9_VERTEX_STREAMS_H

#include "vertex_declaration.h"
#include "pt/vector.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {

class stream_source;

/// One or more vertex streams.
class vertex_streams
{
public:
    vertex_streams(graphics_device& device);

    void use() const;
	void add(std::shared_ptr<stream_source> source);

    void finalize();

    size_t vertex_count() const;

private: // type definitions
	typedef std::vector<std::shared_ptr<stream_source> > container;

private: // data members
    vertex_declaration m_declaration;
    container m_streams;
};

/// Writes \a value into \a dst converted to \a type.
size_t write_to(char* dst, vector4d const& value, D3DDECLTYPE type);

}} // namespace pt::d3d9

#endif
