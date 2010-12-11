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
// threadpool.h: definition of the ThreadPool class

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <vector>

#include "thread.h"

/**
 * Centralized manager of threads that are created.
 * The ThreadPool class manages and handles creating, joining, and keeping track
 * of all active threads.
 */
class ThreadPool {
	public:
		/// Error codes for various methods
		enum Error { NoError=0, TooManyThreads=-1, ThreadError=-2 };
		
		/// Constructs an empty thread pool
		ThreadPool();

		/// Frees any thread resources.
		~ThreadPool();
		
		/**
		 * Creates and detaches a joinable thread.
		 * The thread will execute the provided routine function with an optional
		 * parameter for said routine. The exact format of the routine declaration is
		 * provided in the thread.h header file, and is independent of the operating system
		 * that the code is compiled under.
		 *
		 * @param routine The thread routine to execute.
		 * @param param Optional parameter to pass to the routine function.
		 *
		 * @return A return code upon completion.
		 */
		Error createThread(Threads::ThreadFunc routine, void *param);
		
		/**
		 * Returns the last error to have occurred from a previous operation.
		 */
		int lastError() const { return m_LastError; }
	
	private:
		/// Vector of all threads, active or dead.
		std::vector<Threads::thread_t> m_Threads;
		
		/// Error code of the last error to have occurred.
		int m_LastError;
};

#endif
