#ifndef PT_WIN32_DIALOG_BOX_ERROR_H
#define PT_WIN32_DIALOG_BOX_ERROR_H

#include "../exceptions/contextual_exception.h"

namespace pt { namespace win32 {

/// Exception class for windows common dialog box errors.
class dialog_box_error : public exceptions::contextual_exception
{
public:
    dialog_box_error(string_wrapper const& text);
};

}} // namespace pt::win32

#endif
