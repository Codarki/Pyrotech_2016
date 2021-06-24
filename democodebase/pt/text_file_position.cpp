#include "text_file_position.h"
#include "pt/none.h"

namespace pt {

text_file_position::text_file_position()
: m_row(none<size_t>())
, m_column(none<size_t>())
{
}

text_file_position::text_file_position(std::string const& path, size_t row,
    size_t column)
: m_path(path)
, m_row(row)
, m_column(column)
{
}

std::string text_file_position::path() const
{
    return m_path;
}

size_t text_file_position::row() const
{
    return m_row;
}

size_t text_file_position::column() const
{
    return m_column;
}

} // namespace pt

