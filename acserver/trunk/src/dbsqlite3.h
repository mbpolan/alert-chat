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

#include "database.h"

class QueryResult {
	public:
		QueryResult() {
			table=NULL;
			error=NULL;
		}
		
		~QueryResult() {
			sqlite3_free_table(table);
			if (error) free(error);
		}
		
		char **table;
		char *error;
		int rows;
		int cols;
};

class DatabaseSQLite3: public Database {
	public:
		DatabaseSQLite3();
		
		bool open(const std::string &file);
		bool close();
		
		QueryResult* query(const std::string &sql);
	
	private:
		sqlite3 *m_Handle;
};

#endif
