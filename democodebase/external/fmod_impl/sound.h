#ifndef EXTERNAL_FMOD_IMPL_SOUND_H
#define EXTERNAL_FMOD_IMPL_SOUND_H

#include "native.h"
#include "pt/std/memory.h"

namespace fmod_impl {

class system;

class system_child
{
protected:
	system_child(std::shared_ptr<fmod_impl::system> system);
    ~system_child();

	std::shared_ptr<fmod_impl::system const> system() const;
	std::shared_ptr<fmod_impl::system> system();

private: // data members
	std::shared_ptr<fmod_impl::system> m_system;
};

class sound : system_child
{
public:
    ~sound();

    FMOD::Channel* play();
    FMOD::Sound* native();

public:
	sound(std::shared_ptr<fmod_impl::system> system, FMOD::Sound* native);

private: // helper functions
    void release();

private: // data members
    FMOD::Sound* m_native;
};

} // namespace fmod_impl

#endif