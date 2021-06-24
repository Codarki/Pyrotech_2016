#ifndef PT_EXCEPTIONS_FILE_ERROR_H
#define PT_EXCEPTIONS_FILE_ERROR_H

#include "contextual_exception.h"
#include "pt/path.h"

namespace pt { namespace exceptions {

/// Error in file. Describes the error with file, row and column.
class file_error : public contextual_exception
{
public:
    file_error(pt::path const& path, std::string const& text);
    file_error(pt::path const& path, format const& text);
};

enum native_file_error_code
{
    native_file_reading_failed,
    native_file_reading_end_of_file,
    native_file_error_code_count
};

class native_file_error : public contextual_exception
{
public:
    native_file_error(const pt::path& path, native_file_error_code code);
};

}} // namespace pt::exceptions

#endif
