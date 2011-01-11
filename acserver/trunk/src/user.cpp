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

#include <sstream>

#include "database.h"
#include "user.h"
#include "usermanager.h"

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
void User::sendUserStatusUpdate(const std::string &user, int status) {
	m_ComProtocol->sendUserStatusUpdate(user, status);
}

void User::addFriend(const std::string &friendName) {
	// see if this username is already a friend
	for (StringList::iterator it=m_FriendList.begin(); it!=m_FriendList.end(); ++it) {
		if ((*it)==friendName) {
			sendServerMessage("This user is already on your friend list!");
			return;
		}
	}

	// stupidity check: the user tries to add himself
	if (friendName==m_Username) {
		sendServerMessage("You can't add yourself to your friend list!");
		return;
	}

	m_FriendList.push_back(friendName);

	// add the friend to the database
	Database *db=Database::getHandle();
	if (db->open()) {
		// check if the friend exists
		std::string sql="SELECT * FROM users WHERE ";
		sql+=db->compareFoldCase("username", friendName);
		Database::QueryResult friendRes=db->query(sql);

		if (friendRes.rowCount()>0) {
			// resolve this user's id
			sql="SELECT * FROM users WHERE ";
			sql+=db->compareFoldCase("username", m_Username);
			Database::QueryResult thisRes=db->query(sql);

			// add the friend to the user's list
			std::stringstream ss;
			ss << "INSERT INTO friendlists VALUES(NULL,";
			ss << thisRes.rowAt(0)[0] << ",";
			ss << friendRes.rowAt(0)[0] << ")";

			db->query(ss.str());

			// finally send that user's status
			sendUserStatusUpdate(friendName, UserManager::defaultManager()->isUserOnline(friendName));
		}

		// inform the user that such a username doesn't exist
		else {
			std::string msg="The username '";
			msg+=friendName;
			msg+="' does not exist.";

			sendServerMessage(msg);
		}
	}

	delete db;
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

void User::addBlockedUser(const std::string &username) {
	// check if the user tried to block himself
	if (username==m_Username)
		return;

	// first see if this username is in this user's friend list
	for (StringList::iterator it=m_FriendList.begin(); it!=m_FriendList.end(); ++it) {
		if ((*it)==username) {
			it=m_FriendList.erase(it);
			break;
		}
	}

	// add the username to the blocked list
	m_BlockList.push_back(username);

	// update the database
	Database *db=Database::getHandle();
	if (db->open()) {
		// resolve the blocked user's id
		std::string sql="SELECT * FROM users WHERE ";
		sql+=db->compareFoldCase("username", username);
		Database::QueryResult blockRes=db->query(sql);

		// resolve this user's databse id
		sql="SELECT * FROM users WHERE ";
		sql+=db->compareFoldCase("username", m_Username);
		Database::QueryResult thisRes=db->query(sql);

		// add an entry into the block list table in the database
		sql="INSERT INTO blocklists VALUES(NULL,";
		sql+=thisRes.rowAt(0)[0];
		sql+=",";
		sql+=blockRes.rowAt(0)[0];
		sql+=")";
		db->query(sql);

		db->close();

		// send a status update for this user
		sendUserStatusUpdate(username, Blocked);
	}

	delete db;
}

void User::removeBlockedUser(const std::string &username) {

}

bool User::isBlocking(const std::string &username) const {
	if (m_BlockList.empty())
			return false;

	for (StringList::const_iterator it=m_BlockList.begin(); it!=m_BlockList.end(); ++it) {
		if ((*it)==username)
			return true;
	}

	return false;
}
