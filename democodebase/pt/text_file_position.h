#pragma once
#ifndef PT_TEXT_FILE_POSITION_H
#define PT_TEXT_FILE_POSITION_H

#include "pt/path.h"

namespace pt {

/// Contains position in a text file.
class text_file_position
{
public:
    text_file_position();
    text_file_position(std::string const& path, size_t row, size_t column);

    /// \return pt::path of the file.
    std::string path() const;
    /// \return Row number in the file. (line number)
    size_t row() const;
    /// \return Column number in the file.
    size_t column() const;

private: // data members
    std::string m_path;
    size_t m_row;
    size_t m_column;
};

} // namespace pt

#endif
