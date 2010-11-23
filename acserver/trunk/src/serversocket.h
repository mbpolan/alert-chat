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
// serversocket.h: definition of the ServerSocket class

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <iostream>
#include <stdexcept>

#include "definitions.h"

class ServerSocket {
	public:
		class Exception: public std::runtime_error {
			public:
				Exception(const std::string &msg): std::runtime_error(msg) { };
		};
	
	public:
		ServerSocket();
		
		void close();
		void bind(char *port) throw(Exception&);
		void listen() throw(Exception&);
		int accept(std::string &clientIp);
		
	private:
		Socket m_Socket;
};

#endif
