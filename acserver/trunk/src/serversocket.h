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

/**
 * Encapsulation of low level socket functions for portability.
 * This class provides a high level C++ API to the low level socket functions, independent
 * of the operating system this code is compiled on. Note that currently, only BSD sockets
 * are supported, so Windows performance may not be as good as it should be. Support for
 * both IPv4 and IPv6 is implemented.
 *
 * The ServerSocket class provides methods to create, bind, listen on, and accept incoming
 * client connections. Error reporting is done by exceptions, which are thrown for the bind()
 * and listen() methods. The internal backlog of connections is assumed to be a constant defined
 * in the code.
 */
class ServerSocket {
	public:
		/**
		 * Exception thrown by the ServerSocket class upon encountering an unrecoverable error.
		 */
		class Exception: public std::runtime_error {
			public:
				/**
				 * Constructs an exception with the given error message.
				 */
				Exception(const std::string &msg): std::runtime_error(msg) { };
		};
	
	public:
		/**
		 * Constructs an empty and invalid server socket.
		 */
		ServerSocket();
		
		/**
		 * Closes this server socket, disallowing all future client connections.
		 */
		void close();

		/**
		 * Tries to bind this socket to the given address and port.
		 */
		void bind(const std::string &ip, int port) throw(Exception&);

		/**
		 * Begins listening on the server socket.
		 */
		void listen() throw(Exception&);

		/**
		 * Waits for an accepts new client connections.
		 * Client IP address are then stored in the first parameter for future reference.
		 *
		 * @param clientIp The client's IP address.
		 * @return A socket file descriptor for the client.
		 */
		int accept(std::string &clientIp);
		
	private:
		/// The socket the server listens on.
		Socket m_Socket;
};

#endif
