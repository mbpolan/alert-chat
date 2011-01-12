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

bool UserManager::isUserOnline(const std::string &username) const {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	// find out of the user is in the list of online clients
	bool online=(m_Users.find(username)!=m_Users.end());

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	return online;
}

void UserManager::addUser(User *user) {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	broadcastUserStatus(user, User::Online);

	// set the block status on all blocked users
	StringList blocked=user->blockedUsers();
	for (StringList::iterator it=blocked.begin(); it!=blocked.end(); ++it)
		user->sendUserStatusUpdate((*it), User::Blocked);

	// send an initial friend list status update as well
	StringList friends=user->friends();
	for (StringList::iterator it=friends.begin(); it!=friends.end(); ++it) {
		std::string friendName=(*it);

		if (m_Users.find(friendName)!=m_Users.end() && !m_Users[friendName]->isBlocking(user->username()))
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

	// iterate over list of online users and systematically kick each one of them
	for (std::map<std::string, User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it)
			(*it).second->kick();

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::deliverTextMessageTo(const std::string &sender, const std::string &who, const std::string &message) {
	lock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);

	// see if the user is online, and if so, send him the text message assuming that neither
	// party is blocking the other
	if (m_Users.find(who)!=m_Users.end() &&
		!m_Users[who]->isBlocking(sender) && !m_Users[sender]->isBlocking(who))
		m_Users[who]->sendTextMessage(sender, message);

	unlock(&Threads::g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::broadcastUserStatus(User *user, User::Status status) {
	// alert all other clients who have this user added as a friend
	for (std::map<std::string,User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it) {
		User *client=(*it).second;
		StringList friends=client->friends();

		// iterate over all the friends on this user's list
		for (StringList::const_iterator friendIt=friends.begin(); friendIt!=friends.end(); ++friendIt) {
			std::string fname=(*friendIt);

			// so this client has the user as a friend; make sure he is also not blocking him
			if (fname==user->username() && !client->isBlocking(user->username())) {
				client->sendUserStatusUpdate(user->username(), status);
				break;
			}
		}
	}
}
