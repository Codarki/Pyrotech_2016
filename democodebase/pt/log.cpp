#include "log.h"
#include "verbose.h"
#include "win32.h"
#include "string_wrapper.h"
#include "string_wrapper_builder.h"
#include "format.h"
#include <cassert>
#include <locale>

namespace pt { namespace logging {

namespace {
    size_t g_indent_level = 0;
    bool g_last_was_endline = true;
    bool g_use_tabs = false;
    size_t g_indent_spaces = 2;
} // anonymous namespace

namespace detail {
    void log(std::string const& text, bool endline)
    {
        //assert(text.length() < 1000);
        //for (std::string::const_iterator i = text.begin(); i != text.end(); ++i)
        //{
        //    assert(std::isprint(*i, std::locale()));
        //}

        std::string t;
        t.reserve(text.size() + g_indent_level * 4 + 2);
        if (g_last_was_endline)
        {
            for (size_t i = 0; i < g_indent_level; ++i)
            {
                if (g_use_tabs)
                {
                    t += "\t";
                }
                else
                {
                    for (size_t j = 0; j < g_indent_spaces; ++j)
                        t += " ";
                }
            }
        }
        g_last_was_endline = endline;

        t += text;
        if (endline)
            t += "\r\n";

        pt::verbose(t);

        if (verbose_enabled())
            OutputDebugString(t.c_str());
    }
} // namespace detail

scoped_indent::scoped_indent()
{
    ++g_indent_level;
}
scoped_indent::~scoped_indent()
{
    --g_indent_level;
    assert(g_indent_level >= 0);
}

} // namespace logging

void log(char const* text)
{
    logging::detail::log(std::string(text), true);
}

void log(format const& fmt)
{
    logging::detail::log(fmt.string(), true);
}

void log(format&& fmt)
{
    logging::detail::log(fmt.move_string(), true);
}

void log(string_wrapper const& text)
{
    logging::detail::log(text.string(), true);
}

void log(string_wrapper&& text)
{
    logging::detail::log(text.string(), true);
}

void log(string_wrapper_builder&& builder)
{
    logging::detail::log(builder.build().string(), true);
}

} // namespace pt
