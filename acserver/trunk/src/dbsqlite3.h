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

/**
 * Provides a driver for SQLite3 databases.
 * Data is read from the database file provided in the server.conf configuration
 * file. The sqlite3 library is required to compile this driver, at least version
 * 3.7 or newer. Information regarding said library can be found on the official
 * project webpage at http://www.sqlite.org.
 */
class DatabaseSQLite3: public Database {
	public:
		/**
		 * Constructs the driver for SQLite3 databases.
		 * The only parameter is a path to the database file, which should be
		 * loaded upon calling open().
		 *
		 * @param file Path to SQLite3 database file.
		 */
		DatabaseSQLite3(const std::string &file);

		/**
		 * Frees any resources associated with the SQLite3 database.
		 */
		~DatabaseSQLite3();
		
		/**
		 * Attempts to open the given databasefile.
		 *
		 * @return True, if the database file could be opened, false otherwise.
		 */
		bool open();

		/**
		 * Closes and releases resources related to the SQLite3 database file.
		 */
		bool close();
		
		/**
		 * Executes a query on the SQLite3 database.
		 * Note that SQLite3 accepts only a small subset of SQL language normally
		 * encountered in databases such as MySQL.
		 *
		 * @param sql The query to execute.
		 * @return Query object storing the result.
		 */
		Database::QueryResult query(const std::string &sql);

		/**
		 * Fetches the friend list for the given username.
		 *
		 * @param username The name of the user for whom to fetch a friend list.
		 * @return Vector of usernames, potentially empty if the user has no saved friends.
		 */
		StringList getFriendList(const std::string &username);

		/**
		 * Implements a case insensitive comparison of table columns and values.
		 *
		 * @param column The table column name
		 * @param str The string to compare against
		 * @return A formatted SQL statement.
		 */
		std::string compareFoldCase(const std::string &column, const std::string &str);
	
	private:
		/// The file this database is associated with.
		std::string m_File;

		/// SQLite3 handle to the open file.
		sqlite3 *m_Handle;
};

#endif
