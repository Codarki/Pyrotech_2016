#include "file_input_stream.h"
#include "native_file_input_stream.h"
#include "uncompressed_archive.h"
#include "pt/boost/path.h"

namespace pt {

file_input_stream::file_input_stream(path const& path)
{
    std::string text = path.string();

    for (size_t i = 0; i < text.size(); ++i)
    {
        if (text[i] == '/')
            text[i] = '\\';
    }

    pt::path new_path(text);

    find_from_archives(new_path);

    if (!m_stream)
        m_stream.reset(new native_file_input_stream(new_path));
}

void file_input_stream::read(void* data, size_t bytes)
{
    m_stream->read(data, bytes);
}

void file_input_stream::seek(size_t offset)
{
    m_stream->seek(offset);
}

size_t file_input_stream::position() const
{
    return m_stream->position();
}

size_t file_input_stream::size() const
{
    return m_stream->size();
}

void file_input_stream::find_from_archives(path const& path)
{
    typedef std::list<uncompressed_archive const*> archive_list;
    archive_list archives = pt::archives();

    archive_list::const_iterator i = archives.begin();
    archive_list::const_iterator end = archives.end();
    for (; i != end && !m_stream; ++i)
    {
        m_stream = (*i)->find_file(path);
    }
}

bool immutable_file_exists(path const& path)
{
    typedef std::list<uncompressed_archive const*> archive_list;
    archive_list archives = pt::archives();

    archive_list::const_iterator i = archives.begin();
    archive_list::const_iterator end = archives.end();
    for (; i != end; ++i)
    {
        std::string text = path.string();
        for (size_t j = 0; j < text.size(); ++j)
        {
            if (text[j] == '/')
                text[j] = '\\';
        }
        pt::path new_path(text);

        uncompressed_archive::const_iterator j = (*i)->find_file_info(new_path);
        uncompressed_archive::const_iterator info_end = (*i)->end();
        if (j != info_end)
            return true;
    }

    return pt::exists(path);
}

} // namespace pt
