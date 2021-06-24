#include "shader_utils.h"
#include "pt/config_node.h"
#include "pt/data_stream_content.h"
#include "pt/file_input_stream.h"
#include "pt/format.h"
#include "pt/path.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace shaders {

string_wrapper read_hlsl_from_file(pt::path file_path)
{
    file_input_stream file_stream(file_path);
    data_stream_content data(file_stream);

    char const* ptr = reinterpret_cast<char const*>(data.data());
    std::string text(ptr, data.size());
    assert(text.c_str()[data.size()] == 0);

    std::string hlsl = (format("#line %1% \"%2%\"\n")
        % 1
        % file_path.string()).move_string();

    hlsl += text;
    return hlsl;
};

string_wrapper parse_hlsl(config_node const& config)
{
    std::string hlsl;

    for (config_node::const_iterator i = config.begin(); i != config.end();
        ++i)
    {
        if (i->key() == "hlsl")
        {
            hlsl += (format("#line %1% \"%2%\"\n")
                % (config.position().row() + 2)
                % config.position().path()).move_string();

            hlsl += i->value();
        }
        else if (i->key() == "include_hlsl")
        {
            std::string filename;
            i->parse_child("file", filename);
            pt::path file_path(filename);

            hlsl += read_hlsl_from_file(file_path).string();
        }
    }

    return hlsl;
};

}}} // namespace pt::d3d9::shaders
