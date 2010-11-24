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
// protocol.cpp: implementation of Protocol class

#include "packet.h"
#include "protocol.h"
#include "protspec.h"

Protocol::Protocol(Socket fd) {
	m_Socket=fd;
}

bool Protocol::authenticate() {
	Packet a, p;
	
	// send the authentication requirement packet
	a.addByte(PROT_REQAUTH);
	if (!a.write(m_Socket))
		return false;
	
	// check for the login packet
	if (p.read(m_Socket)) {
		
		// we're expecting the login packet ONLY
		uint8_t header=p.byte();
		if (header==(char) PROT_LOGIN) {
			std::string username=p.string();
			std::string password=p.string();
				
			std::cout << "Username: " << username << ", password: " << password << std::endl;
			
			// TODO: authentication via database or file
			
			return true;
		}
	}
	
	return false;
}

void Protocol::relay() {
	Packet p;
	while(p.read(m_Socket)) {
		
	}
}