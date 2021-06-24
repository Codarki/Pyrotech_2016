#ifndef PT_WIN32_DLL_H
#define PT_WIN32_DLL_H

#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt {
    class path;
}

namespace pt { namespace win32 {

/// Abstract base class for dll's.
class dll
{
public:
    virtual void* get_proc_address(
        std::string const& function_name) const = 0;
    virtual ~dll();
};

namespace detail {
    class native_dll;
}

/// Simple DLL class.
class simple_dll : public dll
{
public:
    simple_dll(path const& path);

public: // interface required by dll
    void* get_proc_address(std::string const& function_name) const;

private: // data members
    std::shared_ptr<detail::native_dll> m_impl;
};

}} // namespace pt::win32

#endif
