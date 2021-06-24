#ifndef PYROTECH_COMMAND_LINE_H
#define PYROTECH_COMMAND_LINE_H

#include "std/string.h"
#include "std/map.h"

namespace pt {

/// Creates a key - value pairs from a continuous text string.
class command_line
{
public:
    /// Generates internal structures.
    command_line(std::string const& text);

    command_line(int argc, char* argv[]);

    /// Check for key existance. \return \c true if \a key option is
    /// specified.
    bool has(std::string const& key);

    /// Sets the \a key option into the \a value. \return \c true if
    /// succeeded.
    bool try_get_value_of(std::string const& key, std::string& value);

    std::string value_of(std::string const& key);

private: // data members
    std::string m_text;
    std::map<std::string,std::string> m_key_values;
};

} // namespace pt

#endif
