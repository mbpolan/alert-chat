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

#include <sstream>

#include "dbsqlite3.h"
#include "logwriter.h"
#include "usermanager.h"
#include "packet.h"
#include "protocol.h"
#include "protspec.h"

Protocol::Protocol(Socket fd) {
	m_Socket=fd;
}

void Protocol::createUserAccount() {
	Packet p;
	if (p.read(m_Socket)!=Packet::Disconnected) {
		// we're expecting registration details ONLY
		uint8_t header=p.byte();
		if (header==PROT_REGISTERACC) {
			uint8_t resultCode;

			// extract details
			std::string firstName=p.string();
			std::string lastName=p.string();
			std::string location=p.string();
			std::string username=p.string();
			std::string password=p.string();

			p.clear();

			// check if this username is free
			Database *db=Database::getHandle();
			if (db->open()) {
				std::string sql="SELECT * FROM users WHERE username='";
				sql+=username;
				sql+="'";
				Database::QueryResult res=db->query(sql);

				// success! create the account
				if (res.rowCount()==0) {
					resultCode=0x0;

					sql="INSERT INTO users VALUES(NULL,'";
					sql+=username;
					sql+="','";
					sql+=password;
					sql+="')";

					db->query(sql);
				}

				// otherwise the username is taken
				else
					resultCode=0x1;
			}

			db->close();
			delete db;

			// let the client know the results
			p.addByte(PROT_REGISTERACC);
			p.addByte(resultCode);
			p.write(m_Socket);
		}
	}
}

bool Protocol::authenticate(std::string &username, std::string &password) {
	Packet a, p;
	
	// send the authentication requirement packet
	a.addByte(PROT_REQAUTH);
	if (!a.write(m_Socket))
		return false;
	
	// check for the login packet
	if (p.read(m_Socket)!=Packet::Disconnected) {
		// we're expecting the login packet ONLY
		uint8_t header=p.byte();
		if (header==(char) PROT_LOGIN) {
			username=p.string();
			password=p.string();
			
			Database *db=Database::getHandle();
			if (!db->open()) {
				LogWriter::output("Protocol: unable to open database: "+db->lastError(), LogWriter::Error);
				delete db;
			}
			
			else {
				std::string sql="SELECT * FROM users WHERE username='";
				sql+=username;
				sql+="' AND password='";
				sql+=password;
				sql+="'";
				
				// query the database and see if the user's credentials were valid
				Database::QueryResult res=db->query(sql);
				if (res.error()) {
					std::string out="Protocol: unable to query database for user "+username;
					out+=": "+db->lastError();

					LogWriter::output(out, LogWriter::Error);
					db->close();
					
					return false;
				}
				
				int rows=res.rowCount();

				db->close();
				delete db;
				
				return (rows!=0);
				
			}
		}
	}
	
	return false;
}

void Protocol::relay() {
	Packet p;
	int rc;
	while(m_Socket>=0 && (rc=p.read(m_Socket))!=Packet::Disconnected) {
		if (rc==Packet::NoError)
			handlePacket(p);

		sendQueuedPackets();
	}
}

void Protocol::disconnect() {
	if (m_Socket>=0)
		closeSocket(m_Socket);
}

void Protocol::handlePacket(Packet &p) {
	char header=p.byte();
	switch(header) {
		case PROT_ADDFRIEND: clientSentAddFriend(p); break;

		case PROT_REMOVEFRIEND: clientSentRemoveFriend(p); break;

		case PROT_TEXTMESSAGE: clientSentTextMessage(p); break;

		default: {
			std::stringstream ss;
			ss << "Protocol: unknown client packet header " << std::ios_base::hex << header;
			LogWriter::output(ss.str(), LogWriter::Error);
		} break;
	}
}

void Protocol::clientSentAddFriend(Packet &p) {
	// target username
	std::string username=p.string();

	UserManager::defaultManager()->addFriendTo(m_User->username(), username);
}

void Protocol::clientSentTextMessage(Packet &p) {
	// recipient
	std::string recipient=p.string();

	// just contains a string
	std::string message=p.string();

	UserManager::defaultManager()->deliverTextMessageTo(m_User->username(), recipient, message);
}

void Protocol::clientSentRemoveFriend(Packet &p) {
	std::string username=p.string();
	m_User->removeFriend(username);
}

void Protocol::sendQueuedPackets() {
	if (m_OutgoingPackets.empty())
		return;

	for (int i=0; i<m_OutgoingPackets.size(); i++)
		m_OutgoingPackets[i].write(m_Socket);

	m_OutgoingPackets.clear();
}

void Protocol::sendTextMessage(const std::string &from, const std::string &msg) {
	Packet p;

	// simple enough: the packet just contains the message
	p.addByte(PROT_TEXTMESSAGE);
	p.addString(from);
	p.addString(msg);

	m_OutgoingPackets.push_back(p);
}

void Protocol::sendServerMessage(const std::string &msg) {
	Packet p;

	p.addByte(PROT_SERVERMESSAGE);
	p.addString(msg);

	m_OutgoingPackets.push_back(p);
}

void Protocol::sendFriendList(const StringList &lst) {
	Packet p;

	// add the header and the length of the friend list
	p.addByte(PROT_FRIENDLIST);
	p.addUint16(lst.size());

	// add each friend's username
	for (StringList::const_iterator it=lst.begin(); it!=lst.end(); ++it)
		p.addString((*it));

	m_OutgoingPackets.push_back(p);
}

void Protocol::sendUserStatusUpdate(const std::string &user, int status) {
	Packet p;

	p.addByte(PROT_STATUSUPDATE);
	p.addString(user);
	p.addUint16(status);

	m_OutgoingPackets.push_back(p);
}
