#ifndef PT_DEBUG_ONLY_H
#define PT_DEBUG_ONLY_H

/// \def PT_DEBUG_ONLY
/// On debug builds, expands x as it is. On non-debug builds, expands to
/// nothing.

#ifdef _DEBUG
    #define PT_DEBUG_ONLY(x) x
#else
    #define PT_DEBUG_ONLY(x)
#endif

#endif
