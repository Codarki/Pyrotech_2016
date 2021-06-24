#include "verbose.h"
#include "file_output_stream.h"
#include "file_openmode.h"
#include "file_type.h"
#include "pt/none.h"
#include "pt/path.h"
#include "pt/win32.h"
#include "pt/string_builder.h"

namespace pt {

namespace {
    static bool first_call = true;
    static bool is_enabled = false;

    size_t global_start_time_ms = none<size_t>();
    size_t global_current_time_ms = none<size_t>();
} // anonymous namespace

void enable_verbose(bool enable)
{
    is_enabled = enable;
}

bool verbose_enabled()
{
    return is_enabled;
}

void verbose(const char* text)
{    
    if (is_enabled)
    {
        // Truncate if first call, otherwise append.
        pt::file_openmode mode = first_call
            ? pt::openmode::truncate : pt::openmode::append;

        global_current_time_ms = ::GetTickCount();

        if (first_call)
        {
            global_start_time_ms = ::GetTickCount();
        }

        size_t elapsed_ms = global_current_time_ms - global_start_time_ms;
        size_t elapsed_seconds = static_cast<size_t>(elapsed_ms / 1000.0f);
        elapsed_ms = (elapsed_ms % 1000);

        std::string real_text = "[" + to_string(elapsed_seconds) + ":" + to_string(elapsed_ms) + "]: " + text;

        first_call = false;

        file_output_stream file("VERBOSE.txt", file_type(true), mode);
        write(file, real_text.c_str());
    }
}

void verbose(std::string const& text)
{
    verbose(text.c_str());
}

} // namespace pt
