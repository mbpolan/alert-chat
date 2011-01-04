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

#include "database.h"
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

void User::sendServerMessage(const std::string &str) {
	m_ComProtocol->sendServerMessage(str);
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

void User::removeFriend(const std::string &username) {
	for (StringList::iterator it=m_FriendList.begin(); it!=m_FriendList.end(); ++it) {
		if ((*it)==username) {
			it=m_FriendList.erase(it);
			break;
		}
	}

	// now remove this user from the database
	Database *db=Database::getHandle();
	if (db->open()) {
		// resolve this user's id
		std::string query="SELECT * FROM users WHERE username='";
		query+=m_Username;
		query+="'";
		Database::QueryResult res=db->query(query);

		std::string thisUser=res.rowAt(0)[0];

		// now resolve the other user's id
		query="SELECT * FROM users WHERE username='";
		query+=username;
		query+="'";
		res=db->query(query);

		std::string toRemove=res.rowAt(0)[0];

		// finally remove this user from the user's friendlist
		query="DELETE FROM friendlists WHERE user=";
		query+=thisUser;
		query+=" AND friend=";
		query+=toRemove;
		db->query(query);

		db->close();
	}

	delete db;

}
