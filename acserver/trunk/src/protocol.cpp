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
// protocol.cpp: implementation of Protocol class

#include "dbsqlite3.h"
#include "usermanager.h"
#include "packet.h"
#include "protocol.h"
#include "protspec.h"

Protocol::Protocol(Socket fd) {
	m_Socket=fd;
}

bool Protocol::authenticate(std::string &username, std::string &password) {
	Packet a, p;
	
	// send the authentication requirement packet
	a.addByte(PROT_REQAUTH);
	if (!a.write(m_Socket))
		return false;
	
	// check for the login packet
	if (p.read(m_Socket)) {
		// we're expecting the login packet ONLY
		uint8_t header=p.byte();
		if (header==(char) PROT_LOGIN) {
			username=p.string();
			password=p.string();
			
			DatabaseSQLite3 db("data.db");
			if (!db.open())
				std::cerr << "Protocol: unable to open database: " << db.lastError() << std::endl;
			
			else {
				std::string sql="SELECT * FROM users WHERE username='";
				sql+=username;
				sql+="' AND password='";
				sql+=password;
				sql+="'";
				
				// query the database and see if the user's credentials were valid
				Database::QueryResult *res=db.query(sql);
				if (res->error()) {
					std::cerr << "Protocol: unable to query database for user " << username << ": ";
					std::cerr << db.lastError() << std::endl;
					db.close();
					
					delete res;
					return false;
				}
				
				int rows=res->rowCount();

				db.close();
				delete res;
				
				return (rows!=0);
				
			}
		}
	}
	
	return false;
}

void Protocol::relay() {
	Packet p;
	while(p.read(m_Socket)) {
		handlePacket(p);
	}
}

void Protocol::handlePacket(Packet &p) {
	char header=p.byte();
	switch(header) {
		case PROT_TEXTMESSAGE: clientSentTextMessage(p); break;

		default: std::cerr << "Protocol: unknown client packet header: " << std::ios_base::hex << header << std::endl; break;
	}
}

void Protocol::clientSentTextMessage(Packet &p) {
	// recipient
	std::string recipient=p.string();

	// just contains a string
	std::string message=p.string();

	UserManager::defaultManager()->deliverTextMessageTo(recipient, message);
}

void Protocol::sendTextMessage(const std::string &msg) {
	Packet p;

	// simple enough: the packet just contains the message
	p.addByte(PROT_TEXTMESSAGE);
	p.addString(msg);

	p.write(m_Socket);
}

void Protocol::sendFriendList(const std::list<std::string> &lst) {
	Packet p;

	// add the header and the length of the friend list
	p.addByte(PROT_FRIENDLIST);
	p.addUint16(lst.size());

	// add each friend's username
	for (std::list<std::string>::const_iterator it=lst.begin(); it!=lst.end(); ++it)
		p.addString((*it));

	p.write(m_Socket);
}
