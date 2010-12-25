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
// usermanager.cpp: implementation of the UserManager class

#include <sstream>

#include "database.h"
#include "usermanager.h"

static UserManager *g_UserManager=NULL;

using namespace Threads;

UserManager::UserManager() {
	g_UserManager=this;
}

UserManager* UserManager::defaultManager() {
	return g_UserManager;
}

void UserManager::addUser(User *user) {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	broadcastUserStatus(user, User::Online);

	// send an initial friend list status update as well
	StringList friends=user->friends();
	for (StringList::iterator it=friends.begin(); it!=friends.end(); ++it) {
		std::string friendName=(*it);
		if (m_Users.find(friendName)!=m_Users.end())
			user->sendUserStatusUpdate(friendName, User::Online);
	}

	m_Users[user->username()]=user;
	
	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::removeUser(User *user) {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	for (std::map<std::string,User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it) {
		if ((*it).second==user) {
			m_Users.erase(it);
			break;
		}
	}

	broadcastUserStatus(user, User::Offline);

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::kickAll() {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	for (std::map<std::string, User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it)
			(*it).second->kick();

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::addFriendTo(const std::string &target, const std::string &username) {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	// stupidity check: the user tries to add himself
	if (target==username) {
		m_Users[target]->sendServerMessage("You can't add yourself to your friend list!");
		unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

		return;
	}

	Database *db=Database::getHandle();
	if (db->open()) {
		// check if the friend exists
		std::string sql="SELECT * FROM users WHERE ";
		sql+=db->compareFoldCase("username", username);
		Database::QueryResult res=db->query(sql);

		if (res.rowCount()>0) {
			std::string friendId=res.rowAt(0)[0];

			sql="SELECT * FROM users WHERE ";
			sql+=db->compareFoldCase("username", target);
			res=db->query(sql);

			// add the friend to the target's list
			std::stringstream ss;
			ss << "INSERT INTO friendlists VALUES(NULL,";
			ss << res.rowAt(0)[0] << ",";
			ss << friendId << ")";

			Database::QueryResult res=db->query(ss.str());

			// and append the username to the client's current friendlist
			m_Users[target]->addFriend(username);
			m_Users[target]->sendFriendList();

			// finally send that user's status
			m_Users[target]->sendUserStatusUpdate(username, m_Users.find(username)!=m_Users.end());
		}

		// inform the user that such a username doesn't exist
		else {
			std::string msg="The username '";
			msg+=username;
			msg+="' does not exist.";

			m_Users[target]->sendServerMessage(msg);
		}
	}

	delete db;

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::deliverTextMessageTo(const std::string &sender, const std::string &who, const std::string &message) {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	if (m_Users.find(who)!=m_Users.end())
		m_Users[who]->sendTextMessage(sender, message);

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::broadcastUserStatus(User *user, User::Status status) {
	// alert all other clients who have this user added as a friend
	for (std::map<std::string,User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it) {
		User *client=(*it).second;
		StringList friends=client->friends();

		// TODO: pick a better data structure than list :/ an O(1) search time would be nice; maybe a hash table?
		for (StringList::const_iterator friendIt=friends.begin(); friendIt!=friends.end(); ++friendIt) {
			std::string fname=(*friendIt);
			if (fname==user->username()) {
				client->sendUserStatusUpdate(user->username(), status);
				break;
			}
		}
	}
}
