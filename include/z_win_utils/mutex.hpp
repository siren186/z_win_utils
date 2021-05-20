#pragma once

#include <windows.h>

namespace zl
{
namespace WinUtils
{

class ZLMutex
{
public:
    virtual ~ZLMutex()
    {
        ::ReleaseMutex(m_handle);
        ::CloseHandle(m_handle);
        m_handle = NULL;
    }

    ZLMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL, BOOL bInitialOwner = FALSE, LPCTSTR lpName = NULL)
    {
        m_handle = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName);
    }

    DWORD Lock(DWORD dwWaitTime = INFINITE)
    {
        return ::WaitForSingleObject(m_handle, dwWaitTime);
    }

    DWORD Unlock()
    {
        return ::ReleaseMutex(m_handle) ? 0 : ::GetLastError();
    }

    bool IsValid(void)
    {
        return (m_handle != NULL);
    }

private:
    ZLMutex& operator=(const ZLMutex&);
    ZLMutex(const ZLMutex&);

private:
    HANDLE m_handle;
};

template<class T>
class ZLMutexGuardImpl
{
public:
    ZLMutexGuardImpl(T& mutex) : m_mutex(mutex)
    {
        m_mutex.Lock();
    }

    virtual ~ZLMutexGuardImpl()
    {
        m_mutex.Unlock();
    }

private:
    T& m_mutex;
};

typedef ZLMutexGuardImpl<ZLMutex> ZLMutexGuard;

}
}
