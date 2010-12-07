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
	lock(&g_Mutexes[MUTEX_USERMANAGER]);

	m_Users[user->username()]=user;

	unlock(&g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::removeUser(User *user) {
	lock(&g_Mutexes[MUTEX_USERMANAGER]);

	for (std::map<std::string,User*>::iterator it=m_Users.begin(); it!=m_Users.end(); ++it) {
		if ((*it).second==user) {
			delete (*it).second;
			m_Users.erase(it);

			break;
		}
	}

	unlock(&g_Mutexes[MUTEX_USERMANAGER]);
}

void UserManager::deliverTextMessageTo(const std::string &who, const std::string &message) {
	lock(&g_Mutexes[MUTEX_USERMANAGER]);

	if (m_Users.find(who)!=m_Users.end())
		m_Users[who]->sendTextMessage(message);

	unlock(&g_Mutexes[MUTEX_USERMANAGER]);
}
