#pragma once
#ifndef PT_INLINE_H
#define PT_INLINE_H

/// \def PT_INLINE
/// Applies a __forceinline directive. This is only a hint for the compiler.
/// On maximum warning level MS VC++ will at least give warning, if unable to
/// inline.

#ifdef WIN32
#define PT_INLINE __forceinline
#else
#define PT_INLINE inline
#endif

#endif
