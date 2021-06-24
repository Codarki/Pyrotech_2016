#include "vector.h"
#include "boost/lexical_cast.h"
#include <boost/tokenizer.hpp>

namespace pt {
namespace {
    template<typename T, int D>
    void parse_vector(vector<T,D>& value, std::string const& text)
    {
        using namespace boost;

        typedef tokenizer<char_separator<char> > tokenizer;
        char_separator<char> sep(" ");
        tokenizer tokens(text, sep);

        size_t count = 0;
        for ( tokenizer::iterator tok_iter = tokens.begin();
            tok_iter != tokens.end(); ++tok_iter)
        {
            assert(count < D);

            std::string token(*tok_iter);
            value[count] = lexical_cast<T>(token);
            ++count;
        }
    }
} // anonymous namespace

void parse_from_text(vector<float,1>& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector2f& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector3f& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector4f& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector<int,1>& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector2i& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector3i& value, std::string const& text)
{
    parse_vector(value, text);
}

void parse_from_text(vector4i& value, std::string const& text)
{
    parse_vector(value, text);
}

} // namespace pt
