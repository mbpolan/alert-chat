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
	broadcastUserStatus(user, LoggedIn);

	// send an initial friend list status update as well
	std::list<std::string> friends=user->friends();
	for (std::list<std::string>::iterator it=friends.begin(); it!=friends.end(); ++it) {
		std::string friendName=(*it);
		if (m_Users.find(friendName)!=m_Users.end())
			user->sendUserStatusUpdate(friendName, LoggedIn);
	}

	m_Users[user->username()]=user;
}

void UserManager::removeUser(User *user) {
	for (std::map<std::string,User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it) {
		if ((*it).second==user) {
			m_Users.erase(it);
			break;
		}
	}

	broadcastUserStatus(user, LoggedOut);

	delete user;
}

void UserManager::deliverTextMessageTo(const std::string &who, const std::string &message) {
	if (m_Users.find(who)!=m_Users.end())
		m_Users[who]->sendTextMessage(message);
}

void UserManager::broadcastUserStatus(User *user, UserStatus status) {
	// alert all other clients who have this user added as a friend
	for (std::map<std::string,User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it) {
		User *client=(*it).second;
		std::list<std::string> friends=client->friends();

		// TODO: pick a better data structure than list :/ an O(1) search time would be nice; maybe a hash table?
		for (std::list<std::string>::const_iterator friendIt=friends.begin(); friendIt!=friends.end(); ++friendIt) {
			std::string fname=(*friendIt);
			if (fname==user->username()) {
				client->sendUserStatusUpdate(user->username(), status);
				break;
			}
		}
	}
}
