#pragma once
#ifndef PT_DATA_INPUT_STREAM_H
#define PT_DATA_INPUT_STREAM_H

#include "bounded_input_stream.h"
#include "stream_fwd.h"
#include "pt/std/memory.h"

namespace pt {

/// Stream from block of raw data.
class data_input_stream : public bounded_input_stream
{
public:
    explicit data_input_stream(
        std::shared_ptr<data_stream_content const>&& content);
    data_input_stream(
        std::shared_ptr<data_stream_content const> const& content,
        size_t position);

    data_input_stream(data_input_stream&& other);
    ~data_input_stream();

    std::shared_ptr<data_stream_content const> const& shared_content() const;

public: // interface required by input_stream
    void read(void* data, size_t bytes);

public: // interface required by bounded_input_stream
    void seek(size_t offset);
    size_t position() const;
    size_t size() const;

private: // noncopyable
    data_input_stream(data_input_stream const&);
    data_input_stream const& operator=(data_input_stream const&);

private: // data members
    std::shared_ptr<data_stream_content const> m_content;
    size_t m_position;
};

inline data_input_stream&& move(data_input_stream& x)
{
    return static_cast<data_input_stream&&>(x);
}

std::shared_ptr<data_input_stream> shared_data_input_stream_from(
    bounded_input_stream&& source);

data_input_stream data_input_stream_from(bounded_input_stream&& source);

std::shared_ptr<data_input_stream>
    shared_data_input_stream_with_shared_content_from(
    data_input_stream const& other);

} // namespace pt

#endif
