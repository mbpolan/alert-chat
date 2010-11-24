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
// server.cpp: implementation of main server program

#include <iostream>

#include "definitions.h"
#include "packet.h"
#include "protocol.h"
#include "protspec.h"
#include "serversocket.h"
#include "threadpool.h"
#include "usermanager.h"

UserManager *g_UserManager;

void* connectionHandler(void *param) {
	Socket fd=(Socket) param;
	Protocol p(fd);
	
	// attempt to authenticate the user, and if so, begin communication relay
	if (!p.authenticate()) {
		// send error message
		Packet ret;
		ret.addByte(PROT_CLIENTMSG);
		ret.addString("Incorrect username or password.");
		
		ret.write(fd);
	}
	
	else
		p.relay();
	
	// close the socket if still necessary
	if (fd>0)
		closeSocket(fd);
	
	std::cout << "Disconnected\n";
	exitThread();
}

int main(int argc, char *argv[]) {
	if (argc<2) {
		std::cout << "Usage: server <port>\n";
		return 0;
	}
	
	// setup a server socket for incoming connections
	ServerSocket sock;
	try {
		sock.bind(argv[1]);
		sock.listen();
	}
	
	catch (ServerSocket::Exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		exit(0);
	}
	
	std::cout << "Alert Chat server " << VERSION << " running...\n";
	
	// so far so good; create a thread pool for new connections
	ThreadPool pool(MAX_CONNECTIONS);
	
	// accept incoming connections
	while(1) {
		std::string ip;
		
		// see if this socket is valid
		Socket fd=sock.accept(ip);
		if (fd>0) {
			if (pool.createThread(connectionHandler, (void*) fd)!=ThreadPool::NoError)
				std::cout << "Rejecting connection from " << ip << " (" << pool.lastError() << ")\n";
			else
				std::cout << "Accepted connection from " << ip << std::endl;
			
		}
	}
	
	std::cout << "Alert Chat server shutting down...\n";
	
	// close the socket and join all active threads
	sock.close();
	pool.drain();
	
	return 0;
}
