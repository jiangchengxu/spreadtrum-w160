// --------------------------------------------------------------------------
//
// TSQueue.h
//
/// TSQueue/TSQCriticalSection template.
///
/// @file
//
// Copyright (c) 2006 QUALCOMM Inc.
//
// All Rights Reserved.  QUALCOMM Proprietary
//
// Export of this technology or software is regulated by the U.S. Government.
// Diversion contrary to U.S. law prohibited.
//
// --------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include <queue>
#include <deque>

// --------------------------------------------------------------------------
// TSQueue
//
/// A TSQueue is a thread-safe wrapper around a std::queue. The TSQueue has a
/// mutex data member and a std::queue data member. Every method call that is
/// made to the TSQueue waits for the mutex to be free and then calls the
/// same method on the std::queue. Not all methods are represented in the
/// TSQueue (e.g. a copy constructor). The TSQueue has methods to lock and
/// unlock the mutex so that ownership of the queue can be maintained over
/// critical sections. Make sure to unlock the queue when done with the lock!
// --------------------------------------------------------------------------
template<class _Ty,	class _Container = std::deque<_Ty> >
class TSQueue
{
public:
    typedef _Container container_type;
    typedef typename _Container::value_type value_type;
    typedef typename _Container::size_type size_type;

    TSQueue() : m_mutex(::CreateMutex(NULL,FALSE,NULL)) {
    }

    ~TSQueue() {
        ::CloseHandle(m_mutex);
    }

    void lock() const {
        ::WaitForSingleObject(m_mutex,INFINITE);
    }

    void unlock() const {
        ::ReleaseMutex(m_mutex);
    }

    bool empty() const {
        TSQCriticalSection<_Ty> localLock(*this);
        return m_queue.empty();
    }

    size_type size() const {
        TSQCriticalSection<_Ty> localLock(*this);
        return m_queue.size();
    }

    value_type& front() {
        TSQCriticalSection<_Ty> localLock(*this);
        return m_queue.front();
    }

    const value_type& front() const {
        TSQCriticalSection<_Ty> localLock(*this);
        return m_queue.front();
    }

    value_type& back() {
        TSQCriticalSection<_Ty> localLock(*this);
        return m_queue.back();
    }

    const value_type& back() const {
        TSQCriticalSection<_Ty> localLock(*this);
        return m_queue.back();
    }

    void push(const value_type& _Val) {
        TSQCriticalSection<_Ty> localLock(*this);
        m_queue.push(_Val);
    }

    void pop() {
        TSQCriticalSection<_Ty> localLock(*this);
        m_queue.pop();
    }


private:
    TSQueue(const TSQueue&) {}

private:
    mutable HANDLE m_mutex;
    std::queue<_Ty> m_queue;

// not supported
    //TSQueue& operator=(const std::queue&);
    //bool operator==(const TSQueue&, const TSQueue&);
    //bool operator<(const TSQueue&, const TSQueue&);
};


// --------------------------------------------------------------------------
// TSQCriticalSection
//
/// TSQCriticalSection is a helper class for the TSQueue. The
/// TSQCriticalSection takes a TSQueue as the parameter in its constructor
/// and calls the TSQueue's lock method. The destructor calls the TSQueue's
/// unlock method. This class allows a TSQueue to remain locked while the
/// TSQCriticalSection is in scope. When the TSQCriticalSection goes out of
/// scope, regardless of the exit path, the TSQueue is unlocked.
// --------------------------------------------------------------------------
template<class _Ty>
class TSQCriticalSection
{
public:
    TSQCriticalSection(const TSQueue<_Ty>& q) : m_q(q) {
        m_q.lock();
    }

    ~TSQCriticalSection() {
        m_q.unlock();
    }


private:
    TSQCriticalSection() {}
    const TSQueue<_Ty>& m_q;
};
