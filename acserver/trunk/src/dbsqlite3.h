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
// dbsqlite3.h: declaration of the DatabaseSQLite3 class

#ifndef DBSQLITE3_H
#define DBSQLITE3_H

#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <list>

#include "database.h"

class DatabaseSQLite3: public Database {
	public:
		DatabaseSQLite3(const std::string &file);
		~DatabaseSQLite3();
		
		bool open();
		bool close();
		
		Database::QueryResult* query(const std::string &sql);

		StringList getFriendList(const std::string &username);
	
	private:
		std::string m_File;
		sqlite3 *m_Handle;
};

#endif
