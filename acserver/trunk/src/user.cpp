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
// user.cpp: implementation of User class

#include "user.h"

User::User(const std::string &username, const std::string &password) {
	m_Username=username;
	m_Password=password;

	m_ComProtocol=NULL;
}

void User::kick() {
	m_ComProtocol->disconnect();
}

void User::sendTextMessage(const std::string &from, const std::string &msg) {
	m_ComProtocol->sendTextMessage(from, msg);
}

// sends the user an updated friend list
void User::sendFriendList() {
	m_ComProtocol->sendFriendList(m_FriendList);
}

// sends a friend's online status update
void User::sendUserStatusUpdate(const std::string &user, bool online) {
	m_ComProtocol->sendUserStatusUpdate(user, online);
}

void User::addFriend(const std::string &userName) {
	m_FriendList.push_back(userName);
}
