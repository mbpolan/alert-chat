/***************************************************************************
 *   Copyright (C) 2010 by Michal Polan                                    *
 *   kanadakid@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
// thread.h: encapsulation of various threading APIs under one standard one

#ifndef THREAD_H
#define THREAD_H

/// A collection of thread functions, independent of the underlying operating system.
namespace Threads {

#ifdef __LINUX__
#include <iostream>
#include <pthread.h>

typedef pthread_t thread_t;
typedef pthread_mutex_t mutex_t;

/// Creates a mutex.
#define createMutex(x) pthread_mutex_init(x, NULL)

/// Destroys a mutex.
#define destroyMutex(x) pthread_mutex_destroy(x)

/// Locks a mutex.
#define lock(x) pthread_mutex_lock(x)

/// Unlocks a mutex.
#define unlock(x) pthread_mutex_unlock(x)

/// Terminates the current thread.
#define exitThread() pthread_exit(NULL)

typedef void* (*ThreadFunc)(void*);

// global array of mutexes and defines
#define MUTEX_USERMANAGER	0
#define TOTAL_MUTEXES		1

/// Global array of mutexes.
static mutex_t g_Mutexes[TOTAL_MUTEXES];

/**
 * Creates a thread for the given routine, and (optionally) a parameter to pass to the routine
 *
 * @param routine The thread routine.
 * @param param Parameter to pass to the routine.
 * @param status Sets the outcome of the function to this variable.
 * @return A pthread_t handle to the created thread..
 */
static thread_t createThread(ThreadFunc routine, void *param, int &status) {
	thread_t h;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	status=pthread_create(&h, &attr, routine, param);
	
	pthread_attr_destroy(&attr);
	
	return h;
}

/**
 * Joins a thread to the main thread.
 */
static void joinThread(thread_t handle) {
	pthread_join(handle, NULL);
}

#endif // __LINUX__

} // namespace Threads

#endif

