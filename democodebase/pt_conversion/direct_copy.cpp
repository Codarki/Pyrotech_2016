#include "direct_copy.h"
#include "pt/file_input_stream.h"
#include "pt/file_output_stream.h"
#include "pt/path.h"

namespace pt { namespace conversion {

void direct_copy(pt::path const& src, pt::path const& dst)
{
    pt::path complete_src = pt::system_complete(src);
    pt::path complete_dst = pt::system_complete(dst);

    file_input_stream src_stream(complete_src);
    pt::path dir = pt::system_complete(dst.parent_path());
    pt::create_directories(dir);
    file_output_stream dst_stream(complete_dst);

    std::unique_ptr<unsigned char> file_data(
        new unsigned char[src_stream.size()]);
    
    src_stream.read(file_data.get(), src_stream.size());
    dst_stream.write(file_data.get(), src_stream.size());
}

}} // namespace pt::conversion
