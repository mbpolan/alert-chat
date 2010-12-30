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
// protocol.h: definition of Protocol class

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <list>
#include <vector>

#include "definitions.h"
#include "packet.h"

class User;

/**
 * Network class that handles receiving and handling packets from clients.
 * One protocol object should be assigned to each client, and then activated by a
 * call to the relay() function, which will loop indefinitely, handling incoming packets
 * and if needed, report errors.
 *
 * This class is also responsible for sending out data packets in accordance to the
 * protocol specification, found in the protspec.h file. By sending all data requests through
 * this class, the client can be guaranteed to receive well formatted packets. Further,
 * outgoing packets are placed in a buffer and sent out at once when needed. The buffer is
 * flushed after every time out encountered when reading from the socket.
 */
class Protocol {
	public:
		/**
		 * Constructs a protocol object for the given socket file descriptor.
		 *
		 * @param fd A valid socket descriptor for communication.
		 */
		Protocol(Socket fd);

		/**
		 * Attempts to create a new account with the given details.
		 * If the account cannot be created, either because of a database error,
		 * a conflict of existing user data, etc., the protocol will then inform the
		 * client of the problem.
		 */
		void createUserAccount();

		/**
		 * Sets the user for whom this protocol is responsible for.
		 *
		 * @param user The user who owns this protocol.
		 */
		void setUser(User *user) { m_User=user; }
		
		/**
		 * Attempts to authenticate a client by username and password.
		 * Upon a successful authentication, the username and password parameters are
		 * set to the data the user has sent. This method opens a connection to the database
		 * and tries to see if the provided details are valid.
		 *
		 * @param username The username the client has provided.
		 * @param password The password the client has provided.
		 *
		 * @return True if the user was authenticated, false otherwise.
		 */
		bool authenticate(std::string &username, std::string &password);

		/**
		 * Begins the communication loop, terminating when the client has disconnected or
		 * if an error has occurred.
		 */
		void relay();

		/**
		 * Disconnects the client from the server.
		 */
		void disconnect();
	
	private:
		/**
		 * Takes a packet and tries to parse its message.
		 *
		 * @param p The packet to inspect.
		 */
		void handlePacket(Packet &p);

		/**
		 * Handler for when a client sends an add friend request.
		 */
		void clientSentAddFriend(Packet &p);

		/**
		 * Handler for when a client has sent the server a text message.
		 */
		void clientSentTextMessage(Packet &p);

		/**
		 * Flushes the outgoing packet buffer.
		 */
		void sendQueuedPackets();

		/**
		 * Sends a text message to the user of this protocol.
		 *
		 * @param from The person who sent the message.
		 * @param msg The string containing the message.
		 */
		void sendTextMessage(const std::string &from, const std::string &msg);

		/**
		 * Sends the client a server-level message.
		 *
		 * @param msg The string containing the message.
		 */
		void sendServerMessage(const std::string &msg);

		/**
		 * Sends a friend list to the user of this protocol.
		 *
		 * @param lst A vector of usernames who this user as designated as friends.
		 */
		void sendFriendList(const StringList &lst);

		/**
		 * Sends another user's status update to the user of this protocol.
		 *
		 * @param user The user for whom this status update applies to.
		 * @param status An integer status code.
		 */
		void sendUserStatusUpdate(const std::string &user, int status);

		/// The socket file descriptor this protocol works on.
		Socket m_Socket;

		/// Vector of packets waiting to be sent out.
		std::vector<Packet> m_OutgoingPackets;

		/// The owner of this protocol.
		User *m_User;

		friend class User;
};

#endif
