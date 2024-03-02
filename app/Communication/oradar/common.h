#ifndef COMMON_H_
#define COMMON_H_

#if defined(_WIN32)
#include "impl\windows\win.h"
#include "impl\windows\win_serial.h"
#elif defined(__GNUC__)
#include "Communication/oradar/unix.h"
#include "Communication/oradar/unix_serial.h"
#else
#error "unsupported target"
#endif
#include <Communication/oradar/thread.h>
#include <Communication/oradar/locker.h>
#include <Communication/oradar/timer.h>

#endif

