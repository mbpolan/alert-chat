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
// serversocket.cpp: implementation of the ServerSocket class

#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sstream>

#include "serversocket.h"

ServerSocket::ServerSocket() {
	m_Socket=0;
}

void ServerSocket::close() {
	::close(m_Socket);
}

void ServerSocket::bind(const std::string &ip, int port) throw (Exception&) {
	m_Socket=socket(PF_INET, SOCK_STREAM, 0);
	if (m_Socket<0)
		throw Exception("Unable to create socket");
	
	// avoid the annoying kernel message
	int yes;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))<0)
		throw Exception("Unable to set socket options");
	
	// populate the socket struct
	struct sockaddr_in addr;
	struct addrinfo hints, *ai;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;
	
	// for the dumb reason that the port has to be an array for the getaddrinfo function
	std::stringstream ss;
	ss << port;

	int ret;
	if (getaddrinfo((ip=="any" ? NULL : ip.c_str()), ss.str().c_str(), &hints, &ai)<0) {
		// do it ourselves
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=(ip=="any" ? INADDR_ANY : inet_addr(ip.c_str()));
		memset(&addr.sin_zero, '\0', sizeof(addr.sin_zero));
		
		ret=::bind(m_Socket, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
	}
	
	else
		ret=::bind(m_Socket, ai->ai_addr, ai->ai_addrlen);
	
	// perform some error checking at this point
	if (ret<0)
		throw Exception("Unable to bind socket to address and port");
}

void ServerSocket::listen() throw(Exception&) {
	if (::listen(m_Socket, 10)<0)
		throw Exception("Unable to listen on socket");
}

int ServerSocket::accept(std::string &clientIp) {
	struct sockaddr_storage client;
	socklen_t size=sizeof(client);
	
	int fd=::accept(m_Socket, (struct sockaddr*) &client, &size);
	
	if (client.ss_family==AF_INET) {
		struct sockaddr_in *data=(struct sockaddr_in*) &client;

		char str[INET_ADDRSTRLEN];
		clientIp=inet_ntop(AF_INET, &data->sin_addr, str, INET_ADDRSTRLEN);
	}
	
	else {
		struct sockaddr_in6 *data=(struct sockaddr_in6*) &client;

		char str[INET6_ADDRSTRLEN];
		clientIp=inet_ntop(AF_INET6, &data->sin6_addr, str, INET6_ADDRSTRLEN);
	}
	
	return fd;
}
