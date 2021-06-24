#ifndef PT_UNCOMPRESSED_ARCHIVE_H
#define PT_UNCOMPRESSED_ARCHIVE_H

#include "path.h"
#include "std/list.h"
#include "std/memory.h"
#include "std/vector.h"

namespace pt {

class bounded_input_stream;

class uncompressed_archive
{
private: // type definitions
    struct file_info
    {
        std::string relative_path;
        size_t size;
        size_t position;
        bool accessed;
    };
    typedef std::vector<file_info> info_container;
public:
    typedef info_container::const_iterator const_iterator;

public:
    uncompressed_archive(pt::path const& path);
    ~uncompressed_archive();

    std::unique_ptr<bounded_input_stream> find_file(path const& path) const;

    const_iterator directory_iterator(path const& path) const;
    const_iterator end() const;

    const_iterator find_file_info(path const& path) const;

private: // data members
    path m_archive_path;
    info_container m_files;
};

std::list<uncompressed_archive const*> const& archives();

} // namespace pt

#endif
