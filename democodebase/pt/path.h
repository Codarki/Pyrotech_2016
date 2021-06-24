#pragma once
#ifndef PT_PATH_H
#define PT_PATH_H

#include "pt/std/string.h"
#include <xlocale>

namespace pt {

class path
{
public:
    typedef wchar_t value_type;
    typedef std::basic_string<value_type> string_type;  
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_type;

    path();
    path(path const& other);
    path(path&& other);
    path(std::string const& s);
    path(char const* s);
    ~path();
    path const& operator=(path const& other);
    path const& operator=(path&& other);

    std::string string() const;
    string_type const& native() const;
    value_type const* c_str() const;

    path& remove_filename();
    path& replace_extension(path const& new_extension = path());
    void swap(path& rhs);

    path root_path() const;
    path root_name() const;
    path root_directory() const;
    path relative_path() const;
    path parent_path() const;
    path filename() const;
    path stem() const;
    path extension() const;

    bool empty() const;
    bool has_root_path() const;
    bool has_root_name() const;
    bool has_root_directory() const;
    bool has_relative_path() const;
    bool has_parent_path() const;
    bool has_filename() const;
    bool has_stem() const;
    bool has_extension() const;
    bool is_absolute() const;
    bool is_relative() const;

private: // data members
    class impl;
    impl* m_impl;
};

inline pt::path&& move(pt::path& path)
{
    return static_cast<pt::path&&>(path);
}

inline bool operator<(const path& lhs, const path& rhs)
{
    return lhs.string() < rhs.string();
}

inline bool operator==(pt::path const& lhs, pt::path::value_type const* rhs)
{
    pt::path::value_type const* l(lhs.c_str());
    while ((*l == *rhs || (*l == L'\\' && *rhs == L'/') || (*l == L'/' && *rhs == L'\\'))
        && *l) { ++l; ++rhs; }
    return *l == *rhs || (*l == L'\\' && *rhs == L'/') || (*l == L'/' && *rhs == L'\\');  
}
inline bool operator==(const path& lhs, const path& rhs)              { return lhs == rhs.c_str(); }
inline bool operator==(const path& lhs, const path::string_type& rhs) { return lhs == rhs.c_str(); }
inline bool operator==(const path::string_type& lhs, const path& rhs) { return rhs == lhs.c_str(); }
inline bool operator==(const path::value_type* lhs, const path& rhs)  { return rhs == lhs; }

path operator/(path const& lhs, path const& rhs);
bool create_directories(path const& p);
bool exists(path const& p);

path system_complete(path const& p);
path absolute(path const& p);
path absolute(path const& p, path const& base);
path initial_path();

} // namespace pt

#endif
