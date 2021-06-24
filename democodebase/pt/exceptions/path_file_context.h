#ifndef PT_EXCEPTIONS_PATH_FILE_CONTEXT_H
#define PT_EXCEPTIONS_PATH_FILE_CONTEXT_H

#include "context.h"
#include "pt/path_fwd.h"

namespace pt { namespace exceptions {

/// 
class path_file_context : public context
{
public:
    path_file_context(pt::path const& path);

public: // interface required by context
    std::string const& text() const;

private: // data members
    std::string m_text;
};

}} // namespace pt::exceptions

#endif
