#ifndef PT_EXCEPTIONS_TEXT_FILE_ERROR_H
#define PT_EXCEPTIONS_TEXT_FILE_ERROR_H

#include "contextual_exception.h"

namespace pt {
    class text_file_position;
} // namespace pt

namespace pt { namespace exceptions {

/// Error in text file. Describes the error with file, row and column.
class text_file_error : public contextual_exception
{
public:
    text_file_error(text_file_position const& position,
        format const& fmt);
};

}} // namespace pt::exceptions

#endif
