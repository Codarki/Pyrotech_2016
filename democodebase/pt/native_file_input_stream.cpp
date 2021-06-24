#include "native_file_input_stream.h"
#include "input_file.h"
#include "path.h"

namespace pt {

native_file_input_stream::native_file_input_stream(path const& path)
: m_file(new input_file(pt::absolute(path)))
{
}

native_file_input_stream::~native_file_input_stream()
{
}

void native_file_input_stream::read(void* data, size_t bytes)
{
    return m_file->read(data, bytes);
}

void native_file_input_stream::seek(size_t offset)
{
    return m_file->seek_position(offset);
}

size_t native_file_input_stream::position() const
{
    return m_file->current_position();
}

size_t native_file_input_stream::size() const
{
    return m_file->size();
}

} // namespace pt
