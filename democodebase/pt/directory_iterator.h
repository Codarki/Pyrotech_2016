#ifndef PT_DIRECTORY_ITERATOR_H
#define PT_DIRECTORY_ITERATOR_H

#include "uncompressed_archive.h"
#include "boost/filesystem/path_fwd.h"
#include "boost/filesystem/operations.h"

namespace pt {

class directory_iterator : public boost::iterator_facade<
    directory_iterator, boost::filesystem::path,
    boost::single_pass_traversal_tag>
{
public:
    /// Default construction yields past-the-end.
    directory_iterator();
    directory_iterator(boost::filesystem::path const& path);

private:
    typedef boost::iterator_facade<
        directory_iterator,
        boost::filesystem::path,
        boost::single_pass_traversal_tag> base_type;

    friend class boost::iterator_core_access;

    base_type::reference dereference() const;
    void increment();

    bool equal(directory_iterator const& rhs) const
    {
        bool this_is_end = is_end();
        bool rhs_is_end = rhs.is_end();

        if (this_is_end && rhs_is_end)
            return true;

        if (this_is_end ^ rhs_is_end)
            return false;

        return dereference() == rhs.dereference();
        //return m_imp == rhs.m_imp;
    }
    bool is_end() const
    {
        if (m_has_archive)
        {
            if (m_u_archive_cur == m_u_archive_end)
                return true;
            return false;
        }
        else
        {
            return m_boost_iter == boost::filesystem::directory_iterator();
        }
    }

private: // data members
    bool m_has_archive;
    boost::filesystem::path m_directory;
    boost::filesystem::directory_iterator m_boost_iter;
    uncompressed_archive::const_iterator m_u_archive_cur;
    uncompressed_archive::const_iterator m_u_archive_end;
    mutable boost::filesystem::path m_current_path;
};

} // namespace pt

#endif
