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
// user.h: definition of the User class

#ifndef USER_H
#define USER_H

#include <iostream>
#include <list>

#include "protocol.h"

class User {
	public:
		User(const std::string &username, const std::string &password);

		void setProtocol(Protocol *p) { m_ComProtocol=p; }

		std::string username() const { return m_Username; }

		// sends this user a text message
		void sendTextMessage(const std::string &msg);

		// sends the user an updated friend list
		void sendFriendList();
	
		void addFriend(const std::string &userName);
		void setFriendList(const std::list<std::string> &lst) { m_FriendList=lst; }
		std::list<std::string> friends() const { return m_FriendList; }
	
	private:
		Protocol *m_ComProtocol;

		std::string m_Username;
		std::string m_Password;
		
		std::list<std::string> m_FriendList;
};

#endif
