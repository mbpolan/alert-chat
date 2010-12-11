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

/**
 * Central class for user management.
 * The UserManager class has the responsibility of synchronizing packets that are
 * sent between users. It also is responsible for handling resources for when a user
 * is added or removed from the server. Most of the server functionality goes through
 * this class, especially actions that involve peer-to-peer communication.
 *
 * The UserManager class allows an object of this class to be set as a default, allowing
 * various parts of other code to access it in a thread-safe way.
 */
class UserManager {
	public:
		/**
		 * Constructs a new user manager.
		 * The first call to this constructor will designate this manager as the default,
		 * and any subsequent calls will override the default manager as well.
		 */
		UserManager();

		/**
		 * Frees any resources associated with this manager.
		 */
		~UserManager();

		/**
		 * Returns the default user manager, or NULL if none have been allocated.
		 */
		static UserManager* defaultManager();

		/**
		 * Adds a user to the pool of connected users.
		 * The user should already be authenticated before adding him or her, otherwise
		 * strange behavior will result for clients who have the user listed a friend.
		 *
		 * @param user The user object to add to the pool.
		 */
		void addUser(User *user);

		/**
		 * Removes a given user from the connected users pool.
		 * Note that this function does NOT free the pointer.
		 *
		 * @param user The user to remove from the pool.
		 */
		void removeUser(User *user);

		/**
		 * Effectively disconnects all users on the server, by removing each
		 * from the internal pool and closing their respective sockets.
		 */
		void kickAll();

		/**
		 * Coordinates sending a text message from one user to another.
		 *
		 * @param sender The username of the user who sent the message.
		 * @param who The username of the user who will receive this message.
		 * @param message The string containing the message to send.
		 */
		void deliverTextMessageTo(const std::string &sender, const std::string &who, const std::string &message);

	private:
		/**
		 * Sends every connected user, who is a friend of this user, a status update involving
		 * the aforementioned client.
		 *
		 * @param user The user in question.
		 * @param status The current status of the user.
		 */
		void broadcastUserStatus(User *user, User::Status status);

		/// Hash map, relating users to their usernames.
		std::map<std::string, User*> m_Users;
};

#endif

