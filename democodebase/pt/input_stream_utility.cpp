#include "input_stream_utility.h"
#include "file_input_stream.h"
#include "data_stream_content.h"
#include "path.h"
#include "pt/std/cassert.h"

namespace pt {

std::string read_text_file(path const& path)
{
    file_input_stream file_stream(path);
    data_stream_content data(file_stream);

    char const* ptr = reinterpret_cast<char const*>(data.data());
    std::string shader_file_text(ptr, data.size());
    assert(shader_file_text.c_str()[data.size()] == 0);

    return shader_file_text;
}

} // namespace pt
