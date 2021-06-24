#include "just_do_it.h"
#include "pt/path.h"
#include "pt/std/string.h"

namespace content { namespace conversion {

namespace {

    void convert(std::string const& source_dir, std::string const&)
    {
        pt::path src = pt::path(source_dir) / pt::path("src-data");
        pt::path dst("data");
        
    }

} // anonymous

void convert_to_demo_binary()
{
    std::string source_dir("intermediate");
    std::string target_dir("data");
    convert(source_dir, target_dir);
}

}} // namespace content::conversion
