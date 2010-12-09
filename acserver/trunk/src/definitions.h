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
// definitions.h: various defines used throughout the code

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>
#include <list>
#include <vector>

// type definitions for cleaner code
typedef std::list<std::string> StringList;
typedef std::vector<std::string> StringVector;

// define the server version
#define VERSION		"0.1a"

// how many connections our server supports
#define MAX_CONNECTIONS		500

#ifdef __LINUX__

// typedef an int as a socket to differentiate
#ifndef Socket
typedef int Socket;
#endif

#define closeSocket(x) close(x)

#endif

#endif
