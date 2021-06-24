#include "command_line.h"
#include "string_builder.h"
#include "pt/exceptions/any_error.h"
#include "pt/format.h"

#pragma warning(disable: 4127)  // conditional expression is constant
#include <boost/tokenizer.hpp>

//#include "verbose.h"

namespace pt {

command_line::command_line(std::string const& text)
: m_text(text)
{
    //boost::tokenizer<> tok(std::string("- + / ? a - d"));

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" ", "-");
    tokenizer tokens(text, sep);
    bool next_is_key   = false;
    bool has_value = false;
    for ( tokenizer::iterator tok_iter = tokens.begin();
        tok_iter != tokens.end(); ++tok_iter)
    {
        if (*tok_iter == "-")
        {
            next_is_key = true;
            continue;
        }
        if (next_is_key)
        {
            // search character =
            std::string key(*tok_iter);
            std::string::size_type pos = key.find("=", 0);
            std::string value;
            if (pos != std::string::npos)
            {
                has_value = true;
            }
            if (has_value)
            {
                value = key.substr(pos+1, key.size());
                key.erase(pos, key.size());
                has_value = false;
            }
            /*pt::verbose((std::string("key: ") + key).c_str() );
            if (value.size())
                pt::verbose( (" value: " + value + "\n").c_str() );
            else
                pt::verbose("\n");*/

            m_key_values[key] = value;
            next_is_key = false;
        }
    }
}

namespace {
    std::string text_from(int argc, char* argv[])
    {
        std::string result;
        for (int i = 1; i < argc; ++i)
        {
            if (i > 1)
                result += " ";
            result += pt::to_lower_case(argv[i]);
        }
        return result;
    }
} // anonymous

command_line::command_line(int argc, char* argv[])
: command_line(text_from(argc, argv))
{
}

bool command_line::has(std::string const& text)
{
    size_t count = m_key_values.count(pt::to_lower_case(text));
    if (count)
    {
        return true;
        //typedef map<std::string, std::string>::iterator iter;
        //iter i = m_key_values.find(text);
    }
    return false;
}

bool command_line::try_get_value_of(std::string const& text, std::string& value)
{
    auto lower_text = pt::to_lower_case(text);
    //std::map<std::string, std::string>::iterator cur = m_key_values.find(text);
    //return cur.second;
    if (has(lower_text))
    {
        value = m_key_values[lower_text];
        return true;
    }
    return false;
}

std::string command_line::value_of(std::string const& key)
{
    auto lower_key = pt::to_lower_case(key);
    if (!has(lower_key))
        throw pt::exceptions::any_error{pt::format("Command line key '%1%' not found.") % key};
    return m_key_values[lower_key];
}

} // namespace pt
