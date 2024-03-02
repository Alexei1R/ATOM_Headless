#ifndef THREAD_H_
#define THREAD_H_

#include "v8stdint.h"
#include "datatype.h"

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <cassert>
#endif

#include "ATOM/atompch.h"

#if defined(__ANDROID__)
#define pthread_cancel(x) 0
#endif

#define CLASS_THREAD(c, x) Thread::ThreadCreateObjectFunctor<c, &c::x>(this)

namespace ordlidar {
    namespace core {
        namespace base {

            using thread_proc_t = void* (*)(void*);

            class Thread {
            public:
                template <class CLASS, int (CLASS::*PROC)()> static Thread
                ThreadCreateObjectFunctor(CLASS* pthis) {
                    return createThread(createThreadAux<CLASS, PROC>, pthis);
                }

                template <class CLASS, int (CLASS::*PROC)()> static void* THREAD_PROC
                createThreadAux(void* param) {
                    return reinterpret_cast<void*>((static_cast<CLASS*>(param)->*PROC)());
                }

                static Thread createThread(thread_proc_t proc, void* param = nullptr) {
                    Thread thread_(proc, param);
#if defined(_WIN32)
                    thread_._handle = reinterpret_cast<_size_t>(_beginthreadex(nullptr, 0,
                                                                  reinterpret_cast<unsigned int (__stdcall*)(void*)>(proc), param, 0, nullptr));
#else
                    assert(sizeof(thread_._handle) >= sizeof(pthread_t));

                    pthread_create(reinterpret_cast<pthread_t*>(&thread_._handle), nullptr, proc, param);
#endif
                    return thread_;
                }

            public:
                explicit Thread() : _param(nullptr), _func(nullptr), _handle(0) {}
                virtual ~Thread() {}
                _size_t getHandle() const {
                    return _handle;
                }
                int terminate() {
#if defined(_WIN32)

                    if (!_handle) {
            return 0;
        }

        if (TerminateThread(reinterpret_cast<HANDLE>(_handle), -1)) {
            CloseHandle(reinterpret_cast<HANDLE>(_handle));
            _handle = 0;
            return 0;
        } else {
            return -2;
        }

#else

                    if (!_handle) {
                        return 0;
                    }

                    return pthread_cancel(reinterpret_cast<pthread_t>(_handle));
#endif
                }
                void* getParam() const {
                    return _param;
                }
                int join(unsigned long timeout = -1) {
                    if (!_handle) {
                        return 0;
                    }

#if defined(_WIN32)

                        switch (WaitForSingleObject(reinterpret_cast<HANDLE>(_handle), timeout)) {
        case WAIT_OBJECT_0:
            CloseHandle(reinterpret_cast<HANDLE>(_handle));
            _handle = 0;
            return 0;

        case WAIT_ABANDONED:
            return -2;

        case WAIT_TIMEOUT:
            return -1;
        }

#else
                    UNUSED(timeout);
                    void* res;
                    int s = -1;
                    s = pthread_cancel(reinterpret_cast<pthread_t>(_handle));

                    if (s != 0) {
                    }

                    s = pthread_join(reinterpret_cast<pthread_t>(_handle), &res);

                    if (s != 0) {
                    }

                    if (res == PTHREAD_CANCELED) {
                        printf("%lu thread has been canceled\n", _handle);
                        _handle = 0;
                    }

#endif
                    return 0;
                }

                bool operator==(const Thread& right) const {
                    return _handle == right._handle;
                }

            protected:
                explicit Thread(thread_proc_t proc, void* param) : _param(param), _func(proc),
                                                                   _handle(0) {}
                void* _param;
                thread_proc_t _func;
                _size_t _handle;
            };

        } // base
    } // core
} // ordlidar

#endif
