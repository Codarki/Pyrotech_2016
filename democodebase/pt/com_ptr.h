#pragma once
#ifndef PYROTECH_COM_PTR_H
#define PYROTECH_COM_PTR_H

namespace pt {

/// Smart pointer for a COM object of type T. Performs automatic reference
/// additions and decrementations. COM pointers destructs when their
/// internal reference count drops to zero. COM pointers are mostly used by
/// microsoft, in the directx interfaces for example.
template<class T>
class com_ptr
{
public:
    /// Construct null pointer of type T
    com_ptr(); // noexcept
    /// Copy from other and add reference count.
    com_ptr(com_ptr const& other);
    /// Move from other.
    com_ptr(com_ptr&& other);
    template<typename U>
    com_ptr(com_ptr<U> const& other);
    ~com_ptr();

    /// Copy operator. Adds and decreases the reference counts.
	com_ptr<T> const& operator=(com_ptr<T> const& other);
    /// Move assign from other.
    com_ptr<T> const& operator=(com_ptr<T>&& other);

    /// Resets this com_ptr. Decreases the reference count.
    void reset();

    /// Releases this \c com_ptr and gives pointer to pointer of type T.
    /// Practical for giving pointer to the wrapped COM pointer as argument
    /// to functions for receiving.
    T** receive();

    typedef T* com_ptr::*unspecified_bool_type;

    /// Enables boolean condition to check for null pointer.
    operator unspecified_bool_type () const
	{
		return m_com_pointer == 0? 0: &com_ptr::m_com_pointer;
	}

    /// Accessor. \return \c const pointer to the wrapped COM object.
    T const* operator-> () const;
    /// Accessor. \return Pointer to the wrapped COM object.
    T*       operator-> ();
    /// Accessor. \return \c const reference to the wrapped COM object.
    T const& operator* () const;
    /// Accessor. Avoid using. \return Pointer to the wrapped COM object.
	T* get() const {return m_com_pointer;}

    /// Pointer equality. \return \c true if the wrapped COM object is the
    /// same as in \a other
	bool operator== (com_ptr<T> const& other) const;
	bool operator!= (com_ptr<T> const& other) const;

private:
    T*  m_com_pointer;
};

//////////////////////////////////////
// TODO move these into com_ptr.inl
template<class T>
com_ptr<T>::com_ptr()
:   m_com_pointer(0)
{
}

template<class T>
com_ptr<T>::com_ptr(com_ptr<T> const& other)
:	m_com_pointer(other.m_com_pointer)
{
	if (m_com_pointer)
		m_com_pointer->AddRef();
}

template<class T>
com_ptr<T>::com_ptr(com_ptr&& other)
: m_com_pointer(other.m_com_pointer)
{
    other.m_com_pointer = nullptr;
}

template<class T>
template<class U>
com_ptr<T>::com_ptr(com_ptr<U> const& other)
:	m_com_pointer(other.m_com_pointer)
{
	if (m_com_pointer)
		m_com_pointer->AddRef();
}

template<class T>
com_ptr<T>::~com_ptr()
{
	if (m_com_pointer)
        m_com_pointer->Release();
}

template<class T>
com_ptr<T> const& com_ptr<T>::operator= (com_ptr<T> const& other)
{
    if (m_com_pointer == other.m_com_pointer)
        return *this;

    if (m_com_pointer)
        m_com_pointer->Release();

    m_com_pointer = other.m_com_pointer;
    if (m_com_pointer)
        m_com_pointer->AddRef();
    return *this;
}

template<class T>
com_ptr<T> const& com_ptr<T>::operator=(com_ptr<T>&& other)
{
    if (m_com_pointer == other.m_com_pointer)
        return *this;

    if (m_com_pointer)
        m_com_pointer->Release();

    m_com_pointer = other.m_com_pointer;
    other.m_com_pointer = nullptr;
    return *this;
}

template<class T>
void com_ptr<T>::reset()
{
    if (m_com_pointer)
        m_com_pointer->Release();
    m_com_pointer = 0;
}

template<class T>
T** com_ptr<T>::receive()
{
    if (m_com_pointer)
        m_com_pointer->Release();

    return &m_com_pointer;
}

/*template<class T>
com_ptr<T>::operator unspecified_bool_type () const
{
    return m_com_pointer == 0? 0: &com_ptr::m_com_pointer;
}
*/

template<class T>
T const* com_ptr<T>::operator->() const
{
    return m_com_pointer;
}
template<class T>
T* com_ptr<T>::operator->()
{
    return m_com_pointer;
}

template<class T>
T const& com_ptr<T>::operator*() const
{
    return *m_com_pointer;
}

template<class T>
bool com_ptr<T>::operator==(com_ptr<T> const& other) const
{
    return m_com_pointer == other.m_com_pointer;
}

template<class T>
bool com_ptr<T>::operator!=(com_ptr<T> const& other) const
{
    return !(m_com_pointer == other.m_com_pointer);
}

} // namespace pt

#endif
