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

class ThreadPool {
	public:
		enum Error { NoError=0, TooManyThreads=-1, ThreadError=-2 };
		
		ThreadPool();
		~ThreadPool();
		
		Error createThread(Threads::ThreadFunc routine, void *param);
		
		int lastError() const { return m_LastError; }
	
	private:
		std::vector<Threads::thread_t> m_Threads;
		
		int m_LastError;
};

#endif
