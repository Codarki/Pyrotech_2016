#include "text_file_error.h"
#include "string_context.h"
#include "pt/text_file_position.h"
#include "pt/format.h"

namespace pt { namespace exceptions {
namespace {
    std::shared_ptr<context> create_context(text_file_position const& position)
    {
        std::string path = position.path();

        std::string text;
        text.reserve(path.size() + 20);

        text = (format("%1% (line %2%, column %3%)")
            % path % position.row() % position.column()).move_string();

        std::shared_ptr<string_context> context(new string_context(text));

        return context;
    }
} // anonymous namespace

text_file_error::text_file_error(text_file_position const& position,
    format const& fmt)
: contextual_exception(create_context(position))
{
    set_text(fmt);
}

}} // namespace pt::exceptions
