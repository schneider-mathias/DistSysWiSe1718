/**--------------------------------------------------------------------------------------------------
// project:	-
// file:	ScopedLock.h
//
// summary:	Wrapper for a scoped lock depending on VS version
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			10.01.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-----------------------------------------------------------------------------------------------**/

#include <mutex>

// scoped_lock should be prefered but requires C++17:
// https://stackoverflow.com/questions/43019598/stdlock-guard-or-stdscoped-lock
#if(_MSC_VER < 1910)
// http://en.cppreference.com/w/cpp/thread/lock_guard
#define SCOPED_LOCK(m) std::lock_guard<std::mutex> lock(m)
#define SCOPED_LOCK_NAMED(n,m) std::lock_guard<std::mutex> n(m)
#else
// http://en.cppreference.com/w/cpp/thread/scoped_lock
#define SCOPED_LOCK(m) std::scoped_lock<std::mutex> lock{m}
#define SCOPED_LOCK_NAMED(n,m) std::scoped_lock<std::mutex> n{m}
#endif