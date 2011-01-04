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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <sys/time.h>

#include "configmanager.h"
#include "definitions.h"
#include "dbsqlite3.h"
#include "logwriter.h"
#include "packet.h"
#include "protocol.h"
#include "protspec.h"
#include "serversocket.h"
#include "threadpool.h"
#include "usermanager.h"

ConfigManager *g_ConfigManager;
UserManager *g_UserManager;

// thread routine for handling a new connection from a user
void* connectionHandler(void *param) {
	Socket fd=*(Socket*) param;
	Protocol p(fd);
	
	// see what the client identifies itself by
	Packet id;
	id.read(fd);

	// identifying byte
	uint8_t ident=id.byte();

	// regular user connection
	if (ident==PROT_CLUSER) {
		// attempt to authenticate the user first
		std::string username, password;
		if (!p.authenticate(username, password)) {
			// send error message
			Packet ret;
			ret.addByte(PROT_SERVERMESSAGE);
			ret.addString("Incorrect username or password.");

			ret.write(fd);
		}
		
		else {
			// allocate a user object for this user
			User *user=new User(username, password);
			user->setProtocol(&p);
			p.setUser(user);

			// get this user's friend list
			Database *db=Database::getHandle();
			if (db->open()) {
				StringList friends=db->getFriendList(username);
				db->close();

				user->setFriendList(friends);
				user->sendFriendList();
			}
			delete db;

			// have the receive call timeout after a given time
			struct timeval tv;
			int rto=g_ConfigManager->valueForKey("internal_recvtimeout").toInt();
			tv.tv_sec=rto/1000;
			tv.tv_usec=(rto%1000)*1000;

			if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(tv))==-1) {
				LogWriter::output("Unable to call setsockopt", LogWriter::Error);
				closeSocket(fd);

				exitThread();
			}

			UserManager::defaultManager()->addUser(user);

			// begin communications relay
			p.relay();

			UserManager::defaultManager()->removeUser(user);

			delete user;
		}
	}
	
	// registration from client
	else if (ident==PROT_CLREGISTER)
		p.createUserAccount();

	else {
		std::stringstream ss;
		ss << "Unknown identification packet: " << (unsigned short) ident;
		LogWriter::output(ss.str(), LogWriter::Error);
	}

	// close the socket if still necessary
	if (fd>0)
		closeSocket(fd);
	
	LogWriter::output("Disconnected user", LogWriter::Access);
	exitThread();
}

int main(int argc, char *argv[]) {
	if (argc>1) {
		// show a help message
		if (strcmp(argv[1], "--help")==0) {
			std::cout << "Usage: acserver [--help] [--version]\n";
			std::cout << "\nThe server loads all configuration data from server.conf. Check ";
			std::cout << "the aforementioned file for some brief documentation.\n\n";
			std::cout << "Website: http://alert-chat.sf.net\n";
		}

		// show the version string
		else if (strcmp(argv[1], "--version")==0)
			std::cout << VERSION << std::endl;

		exit(0);
	}
	
	// load our configuration file
	g_ConfigManager=new ConfigManager("server.conf");
	if (!g_ConfigManager->parse()) {
		std::cout << "Error: unable to open server.conf for parsing!\n";
		exit(1);
	}

	ConfigManager::setDefaultManager(g_ConfigManager);

	// determine which logs to enable
	int logs=0;
	if (g_ConfigManager->valueForKey("enable-access-log").toInt())
		logs |= LogWriter::Access;
	if (g_ConfigManager->valueForKey("enable-error-log").toInt())
		logs |= LogWriter::Error;
	if (g_ConfigManager->valueForKey("enable-master-log").toInt())
		logs |= LogWriter::Master;

	// now initialize the logging subsystem
	LogWriter::initialize(logs);

	// setup a server socket for incoming connections
	ServerSocket sock;
	try {
		sock.bind(g_ConfigManager->valueForKey("ip").toString(),
				  g_ConfigManager->valueForKey("port").toInt());
		sock.listen();
	}
	
	catch (ServerSocket::Exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		exit(1);
	}
	
	LogWriter::output("Alert Chat server running... ", LogWriter::Master);
	
	// so far so good; create a thread pool for new connections
	ThreadPool pool;
	
	// and the corresponding manager for users
	g_UserManager=new UserManager;

	// accept incoming connections
	while(1) {
		std::string ip;
		
		// see if this socket is valid
		Socket fd=sock.accept(ip);
		if (fd>=0) {
			if (pool.createThread(connectionHandler, &fd)!=ThreadPool::NoError) {
				std::stringstream ss;
				ss << "Rejecting connection from " << ip << " (" << pool.lastError() << ")";
				LogWriter::output(ss.str(), LogWriter::Access);
			}

			else
				LogWriter::output(std::string("Accepted connection from ")+ip, LogWriter::Access);
			
		}
	}
	
	LogWriter::output("Alert Chat server shutting down...", LogWriter::Master);
	
	// disconnect all users
	g_UserManager->kickAll();

	// close the socket and join all active threads
	sock.close();
	
	// destroy managers
	delete g_ConfigManager;
	delete g_UserManager;

	return 0;
}
