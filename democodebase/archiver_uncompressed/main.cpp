#include "pt/file_input_stream.h"
#include "pt/file_output_stream.h"
#include "pt/path.h"
#include "pt/boost/path.h"
#include "pt/boost/filesystem/convenience.h"
#include "pt/boost/filesystem/operations.h"
#include "pt/boost/cast.h"
#include "pt/boost/lexical_cast.h"
#include "pt/boost/smart_ptr.h"
#include "pt/log.h"
#include "pt/none.h"

#include <iostream>

using namespace pt;

struct file_info
{
    file_info()
    :   size(none<size_t>())
    ,   position_in_archive(none<size_t>())
    ,   offset_position_in_header(none<size_t>())
    {
    }
    boost::filesystem::path path;
    size_t size;
    size_t position_in_archive;
    size_t offset_position_in_header;
};

void scan_directory(path source, std::vector<file_info>& files)
{
    boost::filesystem::path boost_path(source.string());

    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for (boost::filesystem::directory_iterator itr(boost_path); itr != end_itr; ++itr)
    {
        if (itr->path().filename() == "CVS")
            continue;

        if (boost::filesystem::is_directory(*itr))
        {
            scan_directory(itr->path().string(), files);
            continue;
        }

        file_info info;
        info.path = *itr;
        info.size = numeric_cast<size_t>(boost::filesystem::file_size(*itr));

        files.push_back(info);
    }
}

int main(int argc, char* argv[])
{
    argc;
    argv;

    path source("data");
    path target("data.dat");

    if (!pt::exists(source))
    {
        //log("data not found");
        std::cout << "data not found\n";
        return 0;
    }

    {
        path dir = pt::system_complete(target.parent_path());
        if (!dir.empty())
            pt::create_directories(dir);
    }

    std::vector<file_info> files;

    {
        //log("Scanning directory " + source.string() + " ...");
        //logging::scoped_indent indent;

        std::cout << "Scanning directory " + source.string() + " ...\n";

        scan_directory(source, files);

        size_t total_files = 0;
        size_t total_bytes = 0;
        for (std::vector<file_info>::const_iterator i = files.begin();
            i != files.end(); ++i)
        {
            ++total_files;
            total_bytes += i->size;
        }

        //log(lexical_cast<std::string>(total_files) + "files");
        //log(lexical_cast<std::string>(total_bytes) + "bytes");

        std::cout << "  " + lexical_cast<std::string>(total_files) + " files\n";
        std::cout << "  " + lexical_cast<std::string>(total_bytes) + " bytes\n";
    }

    std::cout << "Writing header...\n";

    file_output_stream output(pt::system_complete(target));

    #define CREATE_ID(a,b,c,d) ((a << 24) + (b << 16) + (c << 8) + d)
    write(output, static_cast<uint32>(CREATE_ID('P', 'T', 'U', 'A')));
    #undef CREATE_ID

    write(output, static_cast<uint32>(files.size()));

    // Write header.
    for (std::vector<file_info>::iterator i = files.begin(); i != files.end();
        ++i)
    {
        // position not known at this point.
        uint32 position = 0;

        write(output, i->path.string());
        write(output, static_cast<uint32>(i->size));

        // remember where to write the position for this file.
        uint32 offset_position = numeric_cast<uint32>(output.current_pos());
        i->offset_position_in_header = offset_position;

        write(output, position);
    }

    // Write archive.

    std::cout << "Writing files...\n";

    for (std::vector<file_info>::const_iterator i = files.begin();
        i != files.end(); ++i)
    {
        // Write back the position for this file.
        size_t position = output.current_pos();
        output.seek(i->offset_position_in_header);
        write(output, static_cast<uint32>(position));
        output.seek(position);

        // Write file.
        file_input_stream src_stream(i->path.string());

        assert(i->size == src_stream.size());
		boost::scoped_ptr<unsigned char> file_data(
            new unsigned char[src_stream.size()]);
    
        src_stream.read(file_data.get(), src_stream.size());

        output.write(file_data.get(), src_stream.size());
    }
}
