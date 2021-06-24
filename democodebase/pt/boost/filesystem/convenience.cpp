#include "convenience.h"

namespace boost
{
  namespace filesystem
  {
    std::string extension_2(const boost::filesystem::path& ph)
    {
      typedef std::string string_type;
      string_type leaf = ph.filename().string();

      string_type::size_type n = leaf.rfind('.');
      if (n != string_type::npos)
        return leaf.substr(n);
      else
        return string_type();
    }

    std::string basename_2(const boost::filesystem::path& ph)
    {
      typedef std::string string_type;
      string_type leaf = ph.filename().string();
      string_type::size_type n = leaf.rfind('.');
      return leaf.substr(0, n);
    }
  }
}
