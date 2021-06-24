#include "conversion.h"
#include "defs.h"
#include "process_file.h"
#include "pt/log.h"
#include "pt/boost/path.h"
#include "pt/boost/filesystem/convenience.h"
#include "pt/boost/filesystem/operations.h"

namespace pt { namespace conversion {

namespace {
    bool g_convert_always = false;
}

void process_directory(
    boost::filesystem::path const& src,
    boost::filesystem::path const& dst,
    boost::filesystem::path const& dst_root)
{
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for (boost::filesystem::directory_iterator itr(src); itr != end_itr; ++itr)
    {
        boost::filesystem::path filename = itr->path().filename();

        if (filename == "CVS" || filename == ".svn")
            continue;

        boost::filesystem::path dst_p(dst / filename);
        if (boost::filesystem::is_directory(*itr))
        {
            process_directory(*itr, dst_p, dst_root);
            continue;
        }
        //std::string filename = itr->leaf();

        std::time_t src_time = last_write_time(*itr);

        if (dst_p.extension() == ".3ds" || dst_p.extension() == ".3DS")
        {
            dst_p.replace_extension(".p3d_scene");
        }

        if (!boost::filesystem::exists(dst_p))
        {
            boost::filesystem::path dest_dir = dst_p.parent_path();

            boost::filesystem::create_directories(dest_dir);

            process_file(itr->path().string(), dst_p.string(), dst_root.string());
            if (boost::filesystem::exists(dst_p))
                last_write_time(dst_p, src_time);
        }
        else
        {
            std::time_t dst_time = last_write_time(dst_p);
            if (src_time != dst_time || g_convert_always)
            {
                process_file(itr->path().string(), dst_p.string(), dst_root.string());
            }
            else
            {
                //log("Skipping conversion for file \""
                //    + dst_p.file_string() + "\"");
            }
            if (boost::filesystem::exists(dst_p))
                last_write_time(dst_p, src_time);
        }
    }
}

void convert(std::string const& source_dir, std::string const& target_dir)
{
    auto src = boost::filesystem::path(source_dir);
    auto dst = boost::filesystem::path(target_dir);

    process_directory(src, dst, dst);
}

void convert(conversion_settings const&)
{
}

}} // namespace pt::conversion
