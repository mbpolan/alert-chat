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
// protspec.h: unified client-server protocol specification

#ifndef PROTSPEC_H
#define PROTSPEC_H

// client identification
#define PROT_CLUSER					0xFF
#define PROT_CLREGISTER				0xFE

// login and logout
#define PROT_LOGIN					0x01
#define PROT_LOGOUT					0x02

// server messages
#define PROT_SERVERMESSAGE			0x10

// authentication
#define PROT_REQAUTH				0x20

// friend lists and status updates
#define PROT_FRIENDLIST				0x25
#define PROT_STATUSUPDATE			0x26
#define PROT_ADDFRIEND				0x27
#define PROT_REMOVEFRIEND			0x28
#define PROT_BLOCKUSER				0x29
#define PROT_UNBLOCKUSER			0x30

// peer to peer communication
#define PROT_TEXTMESSAGE			0x40

// registration
#define PROT_REGISTERACC			0x70

#endif
