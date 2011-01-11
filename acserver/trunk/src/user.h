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
// user.h: definition of the User class

#ifndef USER_H
#define USER_H

#include <iostream>
#include <list>

#include "protocol.h"

/**
 * Data object for a connected user.
 * The User class stores and handles various operations involving this user. For example,
 * after a successful authentication, the class contains the username and password of
 * this user. Sending text messages to this user is done by the provided functions, which
 * wrap the underlying protocol. All protocol operations are in terms of this user; that is,
 * sendTextMessage() for example, will send a text message to THIS user.
 */
class User {
	public:
		/// A user's status.
		enum Status { Offline=0x00, Online=0x01, Blocked=0x02 };

		/**
		 * Constructs a user object for the given username and password.
		 *
		 * @param username The username of this user.
		 * @param password The password of this user.
		 */
		User(const std::string &username, const std::string &password);

		/**
		 * Sets the protocol for this user.
		 * This function MUST be called to ensure proper operation.
		 *
		 * @param p Pointer to a protocol object for this user.
		 */
		void setProtocol(Protocol *p) { m_ComProtocol=p; }

		/**
		 * Removes the user from the server.
		 */
		void kick();

		/**
		 * Returns the username of this user.
		 */
		std::string username() const { return m_Username; }

		/**
		 * Sends this user a text message from another.
		 *
		 * @param from The username of the sender.
		 * @param msg The message string.
		 */
		void sendTextMessage(const std::string &from, const std::string &msg);

		/**
		 * Sends this user a server-level message.
		 *
		 * @param str The message text.
		 */
		void sendServerMessage(const std::string &str);

		/**
		 * Sends this user an updated friend list.
		 */
		void sendFriendList();

		/**
		 * Sends this user a status update of another user.
		 *
		 * @param user The username of the user in question.
		 * @param status The user's status (see the Status enum).
		 */
		void sendUserStatusUpdate(const std::string &user, int status);
	
		/**
		 * Tries to add the given username to a user's friend list.
		 * This function also checks to see if the given username even exists in the
		 * database, and if he doesn't, then it informs the user as such.
		 *
		 * @param username The username of another user to add.
		 */
		void addFriend(const std::string &username);

		/**
		 * Removes the given username from this user's friend list.
		 *
		 * @param username The username of another user to remove.
		 */
		void removeFriend(const std::string &username);

		/**
		 * Sets the friend list for this user.
		 *
		 * @param lst A list of usernames who are this user's friends.
		 */
		void setFriendList(const StringList &lst) { m_FriendList=lst; }

		/**
		 * Returns a list of usernames who are designated as friends of this user.
		 */
		StringList friends() const { return m_FriendList; }

		/**
		 * Adds a user to this user's blocked-user list.
		 *
		 * @param username The username of the person to block.
		 */
		void addBlockedUser(const std::string &username);

		/**
		 * Removes a user from this user's blocked list.
		 *
		 * @param username The username of the person to unblock.
		 */
		void removeBlockedUser(const std::string &username);

		/**
		 * Sets this user's list of blocked users.
		 *
		 * @param lst The list of blocked users.
		 */
		void setBlockedList(const StringList &lst) { m_BlockList=lst; }

		/**
		 * Returns this user's list of blocked users.
		 *
		 * @return List of blocked usernames.
		 */
		StringList blockedUsers() const { return m_BlockList; }

		/**
		 * Find out whether a given username is on this user's list of blocked users.
		 *
		 * @return True if the username is blocked, false otherwise.
		 */
		bool isBlocking(const std::string &username) const;
	
	private:
		/// Protocol for this user.
		Protocol *m_ComProtocol;

		/// The user's username.
		std::string m_Username;

		/// The user's password.
		std::string m_Password;
		
		/// The user's list of friends.
		StringList m_FriendList;

		/// The user's list of blocked users
		StringList m_BlockList;
};

#endif
