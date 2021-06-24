#ifndef PT_BOOST_FILESYSTEM_CONVENIENCE_H
#define PT_BOOST_FILESYSTEM_CONVENIENCE_H

#include "../auto_link.h"

#define BOOST_FILESYSTEM_NARROW_ONLY

#pragma warning(push)
#pragma warning(disable: 4245) // conversion from 'x' to 'y', signed/unsigned mismatch
#include <boost/filesystem/convenience.hpp>
#pragma warning(pop)

namespace pt { namespace filesystem {
        //using namespace ::boost::filesystem;
}} // namespace pt::filesystem

//#include "../path.h"
//
//// Since some genius decided to add these definitions derectly into headers,
//// I'll just provide my own versions then.
//#ifndef BOOST_FILESYSTEM_NARROW_ONLY
//#  error no wide char support implemented..
//#endif
//
//namespace boost
//{
//  namespace filesystem
//  {
//    std::string extension_2(const boost::filesystem::path& ph);
//    std::string basename_2(const boost::filesystem::path& ph);
//  }
//}

#endif
