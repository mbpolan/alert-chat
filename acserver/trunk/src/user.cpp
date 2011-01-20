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

#include "configmanager.h"
#include "database.h"
#include "user.h"
#include "usermanager.h"

User::User(const std::string &username, const std::string &password) {
	m_Username=username;
	m_Password=password;
	m_ID=-1;

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
			// add the friend to the user's list
			std::stringstream ss;
			ss << "INSERT INTO friendlists VALUES(NULL,";
			ss << m_ID << ",";
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
		// resolve the other user's id
		int friendId=db->getUserID(username);

		// finally remove this user from the user's friendlist
		std::stringstream ss;
		ss << "DELETE FROM friendlists WHERE user=" << m_ID;
		ss << " AND friend=";
		ss << friendId;

		db->query(ss.str());

		db->close();
	}

	delete db;
}

void User::addBlockedUser(const std::string &username) {
	// check if the user tried to block himself, or if this user is already blocked
	if (username==m_Username || isBlocking(username))
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
		int blockedId=db->getUserID(username);

		// add an entry into the block list table in the database
		std::stringstream ss;
		ss << "INSERT INTO blocklists VALUES(NULL," << m_ID << ",";
		ss << blockedId << ")";
		db->query(ss.str());

		db->close();

		// send a status update for this user
		sendUserStatusUpdate(username, Blocked);

		// if the other user is online, send him a status telling him that we're offline
		UserManager *um=UserManager::defaultManager();
		if (um->isUserOnline(username) && !um->isUserBlocking(username, m_Username))
			um->dispatchUserStatusTo(username, m_Username, Offline);
	}

	delete db;
}

void User::removeBlockedUser(const std::string &username) {
	// stupidity check, user can't block/unblock himself, and the target username must
	// already be on the blocked list
	if (m_Username==username || !isBlocking(username))
		return;

	// remove the user from the block list
	for (StringList::iterator it=m_BlockList.begin(); it!=m_BlockList.end(); ++it) {
		if ((*it)==username) {
			it=m_BlockList.erase(it);
			break;
		}
	}

	// remove the database entry
	Database *db=Database::getHandle();
	if (db->open()) {
		// resolve the target's id
		int userID=db->getUserID(username);

		std::stringstream ss;
		ss << "DELETE FROM blocklists WHERE user=" << m_ID;
		ss << " AND blocked=" << userID;

		db->query(ss.str());
		db->close();

		// tell both users about each others' statuses
		UserManager *um=UserManager::defaultManager();
		if (um->isUserOnline(username) && !um->isUserBlocking(username, m_Username)) {
			// the other user is NOT blocking this client, so tell both of them that each
			// is online
			um->dispatchUserStatusTo(username, m_Username, Online);
			sendUserStatusUpdate(username, Online);
		}

		// otherwise, the other user IS blocking this client
		else
			sendUserStatusUpdate(username, Offline);
	}

	delete db;
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

void User::pushMessageTime(time_t time) {
	if (m_LastMessageTimes.size()==10) {
		m_LastMessageTimes.pop_back();
	}

	m_LastMessageTimes.push_front(time);
}

void User::cleanMessageTimeStack() {
	// remove any message times older than n seconds
	time_t now=time(NULL);
	std::list<time_t>::reverse_iterator it=m_LastMessageTimes.rbegin();
	for (std::list<time_t>::iterator it=m_LastMessageTimes.begin(); it!=m_LastMessageTimes.end(); ++it) {
		time_t msg=(*it);
		if (now-msg>=ConfigManager::manager()->valueForKey("rate-time-unit").toInt()) {
			it=m_LastMessageTimes.erase(it);
		}
	}
}
