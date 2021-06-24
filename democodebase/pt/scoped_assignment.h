#pragma once
#ifndef PT_SCOPED_ASSIGNMENT_H
#define PT_SCOPED_ASSIGNMENT_H

namespace pt {

/// Assigns value on construction and restores the old value on destruction.
template<typename T>
class scoped_assignment
{
public:
    scoped_assignment(T& t, T value)
    :   m_value(t)
    ,   m_old_value(t)
    {
        m_value = value;
    }

    ~scoped_assignment()
    {
        m_value = m_old_value;
    }

private: // noncopyable
    scoped_assignment(scoped_assignment const&);
    void operator=(scoped_assignment const&);

private:
    T& m_value;
    T m_old_value;
};

} // namespace pt

#endif
