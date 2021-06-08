#pragma once

#include <windows.h>
#include <synchapi.h>

namespace zl
{
namespace WinUtils
{

/**
 * ��ȡ������װ��SRWLOCK�����ܽ���̡߳����������⣬�ڼ���ʱ����ǿɱ�̣߳����������������������߳��޷����룩
 */
class ZLSRWLock
{
public:
    ZLSRWLock()
    {
        ::InitializeSRWLock(&m_srwlock);
    }

    void LockShared()
    {
        ::AcquireSRWLockShared(&m_srwlock);
    }

    void UnlockShared()
    {
        ::ReleaseSRWLockShared(&m_srwlock);
    }

    void LockExclusive()
    {
        ::AcquireSRWLockExclusive(&m_srwlock);
    }

    void UnlockExclusive()
    {
        ::ReleaseSRWLockExclusive(&m_srwlock);
    }

private:
    ZLSRWLock& operator=(const ZLSRWLock&);
    ZLSRWLock(const ZLSRWLock&);

private:
    SRWLOCK m_srwlock;
};

class ZLSRWLockSharedGuard
{
public:
    ZLSRWLockSharedGuard(ZLSRWLock& srwlock) : m_srwlock(srwlock)
    {
        m_srwlock.LockShared();
    }

    ~ZLSRWLockSharedGuard()
    {
        m_srwlock.UnlockShared();
    }

private:
    ZLSRWLock& m_srwlock;
};

class ZLSRWLockExclusiveGuard
{
public:
    ZLSRWLockExclusiveGuard(ZLSRWLock& srwlock) : m_srwlock(srwlock)
    {
        m_srwlock.LockExclusive();
    }

    ~ZLSRWLockExclusiveGuard()
    {
        m_srwlock.UnlockExclusive();
    }

private:
    ZLSRWLock& m_srwlock;
};

}
}
