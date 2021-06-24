#include "data_input_stream.h"
#include "data_stream_content.h"
#include "debug_only.h"
#include "pt/std/cassert.h"

namespace pt {

namespace {
    std::shared_ptr<data_stream_content const> create_shared_content(
        bounded_input_stream& source)
    {
        return std::make_shared<data_stream_content const>(source);
    }
} // anonymous namespace

data_input_stream::data_input_stream(
    std::shared_ptr<data_stream_content const>&& content)
: m_content(std::move(content))
, m_position(0)
{
}

data_input_stream::data_input_stream(
    std::shared_ptr<data_stream_content const> const& content,
    size_t position)
: m_content(content)
, m_position(position)
{
}

data_input_stream::data_input_stream(data_input_stream&& other)
: m_content(std::move(other.m_content))
, m_position(other.m_position)
{
}

data_input_stream::~data_input_stream()
{
}

std::shared_ptr<data_stream_content const> const&
    data_input_stream::shared_content() const
{
    return m_content;
}

void data_input_stream::read(void* data, size_t bytes)
{
    m_content->read(m_position, data, bytes);
    m_position += bytes;
}

void data_input_stream::seek(size_t offset)
{
    PT_DEBUG_ONLY(size_t tmp = m_content->size();)
    assert(offset <= tmp);
    m_position = offset;
}

size_t data_input_stream::position() const
{
    return m_position;
}

size_t data_input_stream::size() const
{
    return m_content->size();
}

// free functions

std::shared_ptr<data_input_stream> shared_data_input_stream_from(
    bounded_input_stream&& source)
{
    return std::make_shared<data_input_stream>(
        create_shared_content(source));
}

data_input_stream data_input_stream_from(bounded_input_stream&& source)
{
    return data_input_stream(create_shared_content(source));
}

std::shared_ptr<data_input_stream>
    shared_data_input_stream_with_shared_content_from(
    data_input_stream const& other)
{
    return std::make_shared<data_input_stream>(
        other.shared_content(), other.position());
}

} // namespace pt
