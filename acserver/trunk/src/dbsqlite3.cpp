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
// dbsqlite3.cpp: implementation of the DatabaseSQLite3 class

#include "dbsqlite3.h"

DatabaseSQLite3::DatabaseSQLite3() {
	m_Handle=NULL;
}

bool DatabaseSQLite3::open(const std::string &file) {
	if (sqlite3_open_v2(file.c_str(), &m_Handle, SQLITE_OPEN_READONLY, NULL)!=SQLITE_OK) {
		sqlite3_close(m_Handle);
		return false;
	}
	
	return true;
}

bool DatabaseSQLite3::close() {
	return (sqlite3_close(m_Handle)==SQLITE_OK);
}

QueryResult* DatabaseSQLite3::query(const std::string &sql) {
	QueryResult *res=new QueryResult;
	
	sqlite3_get_table(m_Handle, sql.c_str(), &res->table, &res->rows, &res->cols, &res->error);
	return res;
}
