#include "directory_iterator.h"
#include "boost/path.h"
#include "pt/log.h"
#include "pt/format.h"

namespace pt {

directory_iterator::directory_iterator()
: m_boost_iter(boost::filesystem::directory_iterator())
, m_has_archive(false)
{
}

directory_iterator::directory_iterator(boost::filesystem::path const& path)
:   m_has_archive(false)
{
    pt::path pt_path(path.string());

    m_directory = path;
    log(format("directory_iterator: \"%1%\"") % path.string());
    typedef std::list<uncompressed_archive const*> archive_list;
    //archive_list const& archives = archives();

    if (!archives().empty())
    {
        archive_list::const_iterator cur = archives().begin();
        archive_list::const_iterator end = archives().end();

        for (; cur != end; ++cur)
        {
            m_u_archive_cur = (*cur)->directory_iterator(pt_path);
            m_u_archive_end = (*cur)->end();

            if (m_u_archive_cur != m_u_archive_end)
                break;
        }
        if (cur != end)
            m_has_archive = true;
    }

    if (!m_has_archive)
        m_boost_iter = boost::filesystem::directory_iterator(path);
}

directory_iterator::base_type::reference
    directory_iterator::dereference() const
{
    if (m_has_archive)
    {
        m_current_path = boost::filesystem::path(m_u_archive_cur->relative_path);

		//log(format("directory_iterator->dereference(): m_current_path=\"%1%\"") % m_current_path);

        return m_current_path;
    }

    assert(m_boost_iter != boost::filesystem::directory_iterator());
    m_current_path = *m_boost_iter;
    return m_current_path;
}

void directory_iterator::increment()
{
    if (m_has_archive)
    {
        assert(m_u_archive_cur != m_u_archive_end);

        ++m_u_archive_cur;
		 
		//This affected that subdirectories under material did not work when using archive.
		//JT, 21.04.2011
        /*if (m_u_archive_cur != m_u_archive_end)
        {
            path p(m_u_archive_cur->relative_path);
			log(format("directory_iterator->increment(): m_directory=\"%1%\" p.branch_path()=\"%2%\"") % m_current_path % p.branch_path());
            if (m_directory != p.branch_path())
                m_u_archive_cur = m_u_archive_end;
        }*/
    }
    else
    {
        ++m_boost_iter;
    }
}

} // namespace pt
