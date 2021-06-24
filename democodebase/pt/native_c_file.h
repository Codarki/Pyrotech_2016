#pragma once
#ifndef PT_NATIVE_C_FILE_H
#define PT_NATIVE_C_FILE_H

#include "path_fwd.h"

namespace pt {

class file_type;
enum file_openmode;

class native_c_file
{
public:
    native_c_file(const pt::path& path, const file_type& type, file_openmode mode);
    ~native_c_file();

private: // noncopyable
    native_c_file(const native_c_file&);
    const native_c_file& operator=(const native_c_file&);

private:
    class impl;
    impl* m_impl;
};

} // namespace pt

#endif
