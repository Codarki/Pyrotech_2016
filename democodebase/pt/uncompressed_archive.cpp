#include "uncompressed_archive.h"
#include "data_input_stream.h"
#include "native_file_input_stream.h"
#include "any_error.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/string_wrapper.h"
#include "pt/data_stream_content.h"
#include "pt/std/cassert.h"

namespace pt {

namespace {
    std::list<uncompressed_archive const*> g_archives;
} // anonymous namespace

uncompressed_archive::uncompressed_archive(pt::path const& path)
: m_archive_path(pt::absolute(path))
{
    native_file_input_stream file(m_archive_path);

    uint32 id = pt::read<uint32>(file);

    #define CREATE_ID(a,b,c,d) ((a << 24) + (b << 16) + (c << 8) + d)

    if (id != CREATE_ID('P', 'T', 'U', 'A'))
        throw any_error("invalid archive");

    #undef CREATE_ID

    uint32 file_count = pt::read<uint32>(file);
    m_files.resize(file_count);
	//log("Reading archive...");
    for (size_t i = 0; i < file_count; ++i)
    {
        m_files[i].relative_path = pt::read<string_wrapper>(file).string();
        m_files[i].size = pt::read<uint32>(file);
        m_files[i].position = pt::read<uint32>(file);
		log(m_files[i].relative_path);
    }
    g_archives.push_back(this);
}

uncompressed_archive::~uncompressed_archive()
{
    //for (size_t i = 0; i < m_files.size(); ++i)
    //{
    //    if (!m_files[i].accessed)
    //    {
    //        log(format("file in archive not accessed. path: \"%1%\"") % m_files[i].relative_path);
    //    }
    //}

    std::list<uncompressed_archive const*>::iterator i
        = std::find(g_archives.begin(), g_archives.end(), this);
    assert(i != g_archives.end());

    g_archives.erase(i);
}

std::unique_ptr<bounded_input_stream> uncompressed_archive::find_file(
    path const& path) const
{
    assert(!path.is_absolute());

    info_container::const_iterator i = find_file_info(path);
    if (i == m_files.end())
        return std::unique_ptr<bounded_input_stream>();

    native_file_input_stream file(m_archive_path);

    std::unique_ptr<data_stream_content> data_content(
        new data_stream_content(file, i->position, i->size));
    
    std::unique_ptr<bounded_input_stream> result(
        new data_input_stream(move(data_content)));

    return move(result);
}

uncompressed_archive::const_iterator
    uncompressed_archive::directory_iterator(pt::path const& path) const
{
    const_iterator i = m_files.begin();
    const_iterator end_iter = end();

    for (; i != end_iter; ++i)
    {
        pt::path p(i->relative_path);

		//log(format("archive_directory_iterator path: \"%1%\" branch:\"%2%\"") % path % p.branch_path());

        if (p.parent_path() == path)
            return i;
    }
    return end_iter;
}

uncompressed_archive::const_iterator uncompressed_archive::end() const
{
    return m_files.end();
}

uncompressed_archive::const_iterator
    uncompressed_archive::find_file_info(path const& path) const
{
    info_container::const_iterator i = m_files.begin();
    info_container::const_iterator end = m_files.end();

    for (; i != end; ++i)
    {
        if (i->relative_path == path.string())
            return i;
    }
    return end;
}

std::list<uncompressed_archive const*> const& archives()
{
    return g_archives;
}

} // namespace pt
