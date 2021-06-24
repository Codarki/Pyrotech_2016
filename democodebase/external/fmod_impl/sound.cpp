#include "sound.h"
#include "system.h"
#include "fmod_error.h"
#include <cassert>

namespace fmod_impl {

// system_child

system_child::system_child(std::shared_ptr<fmod_impl::system> system)
:   m_system(system)
{
    assert(m_system);
}

system_child::~system_child()
{
}

std::shared_ptr<fmod_impl::system const> system_child::system() const
{
    return m_system;
}

std::shared_ptr<fmod_impl::system> system_child::system()
{
    return m_system;
}

// sound

sound::~sound()
{
    release();
}

FMOD::Channel* sound::play()
{
    bool paused = true;
    FMOD::Channel* channel = 0;
    check_fmod(system()->native()->playSound(FMOD_CHANNEL_FREE, native(), paused,
        &channel), "FMOD::System::playSound");

    if (!channel)
        throw fmod_error(FMOD_OK, "FMOD::System::createSound");
    return channel;
}


FMOD::Sound* sound::native()
{
    assert(m_native);
    return m_native;
}

sound::sound(std::shared_ptr<fmod_impl::system> system, FMOD::Sound* native)
:   system_child(system)
,   m_native(native)
{
}

void sound::release()
{
    assert(m_native);
    m_native->release();
}

} // namespace fmod_impl
