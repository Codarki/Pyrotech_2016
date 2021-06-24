#include "path.h"
#include "boost/path.h"
#include "boost/filesystem/convenience.h"

namespace pt {

class path::impl
{
public:
    impl()
    {
    }
    impl(impl const& other)
    : m_path(other.m_path)
    {
    }
    impl(boost::filesystem::path const& p)
    : m_path(p)
    {
    }
    impl(boost::filesystem::path&& p)
    : m_path(std::move(p))
    {
    }
    boost::filesystem::path const& boost_path() const
    {
        return m_path;
    }
    boost::filesystem::path& boost_path()
    {
        return m_path;
    }

private: // data members
    boost::filesystem::path m_path;
};

// path

path::path()
: m_impl(new impl)
{
}

path::path(path const& other)
: m_impl(new impl(*other.m_impl))
{
}

path::path(path&& other)
: m_impl(other.m_impl)
{
    other.m_impl = 0;
}

path::path(std::string const& s)
: m_impl(new impl(boost::filesystem::path(s)))
{
}

path::path(char const* s)
: m_impl(new impl(boost::filesystem::path(s)))
{
}

path::~path()
{
    delete m_impl;
}

path const& path::operator=(path const& other)
{
    m_impl = new impl(*other.m_impl);
    return *this;
}

path const& path::operator=(path&& other)
{
    m_impl = other.m_impl;
    other.m_impl = 0;
    return *this;
}

std::string path::string() const
{
    return m_impl->boost_path().string();
}

pt::path::string_type const& path::native() const
{
    return m_impl->boost_path().native();
}

pt::path::value_type const* path::c_str() const
{
    return m_impl->boost_path().c_str();
}

namespace {
    boost::filesystem::path to_boost(pt::path const& p)
    {
        return boost::filesystem::path(p.string());
    }
    pt::path from_boost(boost::filesystem::path const& p)
    {
        return pt::path(p.string());
    }
}

path& path::remove_filename()
{
    m_impl->boost_path().remove_filename();
    return *this;
}

path& path::replace_extension(path const& new_extension)
{
    m_impl->boost_path().replace_extension(new_extension.string());
    return *this;
}

void path::swap(path& rhs)
{
    boost::filesystem::path p(rhs.string());
    m_impl->boost_path().swap(p);
}

path path::root_path() const { return from_boost(to_boost(*this).root_path()); }
path path::root_name() const { return from_boost(to_boost(*this).root_name()); }
path path::root_directory() const { return from_boost(to_boost(*this).root_directory()); }
path path::relative_path() const { return from_boost(to_boost(*this).relative_path()); }
path path::parent_path() const { return from_boost(to_boost(*this).parent_path()); }
path path::filename() const { return from_boost(to_boost(*this).filename()); }
path path::stem() const { return from_boost(to_boost(*this).stem()); }
path path::extension() const { return from_boost(to_boost(*this).extension()); }

bool path::empty() const { return m_impl->boost_path().empty(); }
bool path::has_root_path() const { return m_impl->boost_path().has_root_path(); }
bool path::has_root_name() const { return m_impl->boost_path().has_root_name(); }
bool path::has_root_directory() const { return m_impl->boost_path().has_root_directory(); }
bool path::has_relative_path() const { return m_impl->boost_path().has_relative_path(); }
bool path::has_parent_path() const { return m_impl->boost_path().has_parent_path(); }
bool path::has_filename() const { return m_impl->boost_path().has_filename(); }
bool path::has_stem() const { return m_impl->boost_path().has_stem(); }
bool path::has_extension() const { return m_impl->boost_path().has_extension(); }
bool path::is_absolute() const { return m_impl->boost_path().is_absolute(); }
bool path::is_relative() const { return m_impl->boost_path().is_relative(); }

// free functions

path operator/(path const& lhs, path const& rhs)
{
    return from_boost(to_boost(lhs) / rhs.string());
}

bool create_directories(const path& p)
{
    return boost::filesystem::create_directories(to_boost(p));
}

bool exists(path const& p)
{
    //
    return boost::filesystem::exists(to_boost(p));
}

path system_complete(path const& p)
{
    boost::filesystem::path const source_path(p.string());
    boost::filesystem::path const boost_path(
        boost::filesystem::system_complete(std::move(source_path)));

    return pt::path(boost_path.string());
}

path absolute(path const& p)
{
    return from_boost(boost::filesystem::absolute(to_boost(p)));
}

path absolute(path const& p, path const& base)
{
    return from_boost(boost::filesystem::absolute(to_boost(p), to_boost(base)));
}

path initial_path()
{
    return from_boost(boost::filesystem::initial_path());
}

} // namespace pt
