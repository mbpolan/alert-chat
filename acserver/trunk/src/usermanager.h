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
// usermanager.h: definition of the UserManager class

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <iostream>
#include <map>

#include "user.h"
#include "thread.h"

class UserManager {
	public:
		UserManager();
		~UserManager();

		static UserManager* defaultManager();

		void addUser(User *user);
		void removeUser(User *user);

		void deliverTextMessageTo(const std::string &who, const std::string &message);

	private:
		// maps user names to user objects
		std::map<std::string, User*> m_Users;
};

#endif

