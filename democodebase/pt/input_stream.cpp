#include "input_stream.h"
#include "string_wrapper.h"

namespace pt {

void read(input_stream& stream, std::string& str)
{
    char c;
    do
    {
        stream.read(&c, sizeof(c));
        if (c != '\0')
            str += c;
    }
    while(c != '\0');
}

void read(input_stream& stream, string_wrapper& str)
{
    read(stream, str.string());
}

string_wrapper read_string(input_stream& stream)
{
    string_wrapper result;
    read(stream, result.string());
    return move(result);
}

} // namespace pt
