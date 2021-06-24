#include "path_file_context.h"
#include "pt/path.h"

namespace pt { namespace exceptions {

path_file_context::path_file_context(path const& path)
: m_text(path.string())
{
}

std::string const& path_file_context::text() const
{
    return m_text;
}

}} // namespace pt::exceptions
