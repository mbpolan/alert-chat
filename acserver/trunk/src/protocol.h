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
// protocol.h: definition of Protocol class

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <list>
#include <vector>

#include "definitions.h"
#include "packet.h"

class User;

class Protocol {
	public:
		Protocol(Socket fd);

		void setUser(User *user) { m_User=user; }
		
		bool authenticate(std::string &username, std::string &password);
		void relay();
		void disconnect();
	
	private:
		void handlePacket(Packet &p);
		void clientSentTextMessage(Packet &p);

		void sendQueuedPackets();
		void sendTextMessage(const std::string &msg);
		void sendFriendList(const StringList &lst);
		void sendUserStatusUpdate(const std::string &user, int status);

		Socket m_Socket;
		std::vector<Packet> m_OutgoingPackets;

		User *m_User;

		friend class User;
};

#endif
