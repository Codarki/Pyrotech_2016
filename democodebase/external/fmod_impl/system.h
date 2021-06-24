#ifndef EXTERNAL_FMOD_IMPL_SYSTEM_H
#define EXTERNAL_FMOD_IMPL_SYSTEM_H

#include "pt/boost/enable_shared_from_this.h"
#include "native.h"
#include "pt/path_fwd.h"
#include "pt/std/memory.h"

namespace fmod_impl {

class sound;

class system : public std::enable_shared_from_this<system>
{
public:
    system();
    ~system();

    std::shared_ptr<sound> create_sound(pt::path const& path);

    void update();

    FMOD::System* native();

private: // data members
    FMOD::System* m_native;
};

} // namespace fmod_impl

#endif