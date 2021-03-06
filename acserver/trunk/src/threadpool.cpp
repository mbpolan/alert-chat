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
// threadpool.cpp: implementation of the ThreadPool class

#include "threadpool.h"

using namespace Threads;

ThreadPool::ThreadPool() {
	m_LastError=0;

	for (int i=0; i<TOTAL_MUTEXES; i++)
		createMutex(&g_Mutexes[i]);
}

ThreadPool::~ThreadPool() {
	for (int i=0; i<TOTAL_MUTEXES; i++)
		destroyMutex(&g_Mutexes[i]);
}

ThreadPool::Error ThreadPool::createThread(ThreadFunc routine, void *param) {
	// dispatch a new thread
	thread_t thread=::createThread(routine, param, m_LastError);
	if (m_LastError==0) {
		m_Threads.push_back(thread);
		return NoError;
	}
	
	else
		return ThreadError;
}
