#ifndef PT_LOG_H
#define PT_LOG_H

namespace pt {
    class format;
    class string_wrapper;
    class string_wrapper_builder;
}

namespace pt { namespace logging {

/// Adds indentation level to logging on construct, and decreases it on
/// destruct.
class scoped_indent
{
public:
    /// Add ident.
    scoped_indent();
    /// Decrease ident.
    ~scoped_indent();

    void doxygen_help()
    {
        int arr = 1; arr;
    }
};

} // namespace logging

/// Log \a text into a logfile.
void log(char const* text);
void log(format const& fmt);
void log(format&& fmt);

/// Log \a text into a logfile.
void log(string_wrapper const& text);
void log(string_wrapper&& text);
void log(string_wrapper_builder&& builder);

} // namespace pt

#endif
