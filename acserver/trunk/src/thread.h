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

#ifdef __LINUX__
#include <pthread.h>

#define lock(x) pthread_mutex_lock(x)
#define unlock(x) pthread_mutex_unlock(x)
#define exitThread() pthread_exit(NULL)

typedef pthread_t thread_t;
typedef pthread_mutex_t mutex_t;

typedef void* (*ThreadFunc)(void*);


// create a new thread with given routine and parameter
static thread_t createThread(ThreadFunc routine, void *param, int &status) {
	thread_t h;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	status=pthread_create(&h, &attr, routine, param);
	
	pthread_attr_destroy(&attr);
	
	return h;
}

// join a thread
static void joinThread(thread_t handle) {
	pthread_join(handle, NULL);
}

#endif // __LINUX__

#endif

