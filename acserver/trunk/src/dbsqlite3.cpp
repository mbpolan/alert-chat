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

#include <sstream>

#include "dbsqlite3.h"
#include "logwriter.h"

DatabaseSQLite3::DatabaseSQLite3(const std::string &file) {
	m_File=file;
	m_Handle=NULL;
}

DatabaseSQLite3::~DatabaseSQLite3() {
	if (!m_Handle) {
		sqlite3_close(m_Handle);
		m_Handle=NULL;
	}
}

bool DatabaseSQLite3::open() {
	if (sqlite3_open_v2(m_File.c_str(), &m_Handle, SQLITE_OPEN_READWRITE, NULL)!=SQLITE_OK) {
		sqlite3_close(m_Handle);
		return false;
	}
	
	return true;
}

bool DatabaseSQLite3::close() {
	return (sqlite3_close(m_Handle)==SQLITE_OK);
}

Database::QueryResult DatabaseSQLite3::query(const std::string &sql) {
	char **table;
	char *error;
	int r, c;
	
	if (sqlite3_get_table(m_Handle, sql.c_str(), &table, &r, &c, &error)!=SQLITE_OK) {
		std::string strerr=std::string(error);

		return Database::QueryResult(strerr);
	}

	StringVector colNames;
	Database::QueryResult::RowTable rowData;

	// collect the column names
	for (int i=0; i<c; i++)
		colNames.push_back(table[i]);

	// get the row data; sqlite table result has (r+1)*c elements
	for (int i=c; i<(r+1)*c; i+=c) {
		StringVector row;

		for (int j=0; j<c; j++)
			row.push_back(table[i+j]);

		rowData.push_back(row);
	}

	sqlite3_free_table(table);

	return Database::QueryResult(r, c, rowData, colNames);
}

StringList DatabaseSQLite3::getFriendList(const std::string &username) {
	StringList friends;

	std::stringstream ss;
	ss << "SELECT * FROM users WHERE username=\'" << username << "\'";

	// first get the user's id
	QueryResult res=query(ss.str());
	if (res.error()) {
		std::string out="DatabaseSQLite3: Error while resolving user id for friendlist for user ";
		out+=username+": "+res.errorMessage();

		LogWriter::output(out, LogWriter::Error);
		return friends;
	}

	// now get all the friend list entries for this user
	// we got a row from the users table: <id>,<username>,<password>, the key is index 0
	ss.str("");
	ss << "SELECT * FROM friendlists WHERE user=" << res.rowAt(0)[0];

	res=query(ss.str());
	if (res.error()) {
		std::string out="DatabaseSQLite3: Error while getting friendlist for user ";
		out+=username+": "+res.errorMessage();

		LogWriter::output(out, LogWriter::Error);
		return friends;
	}

	ss.str("");

	for (int i=0; i<res.rowCount(); i++) {
		// fetching from the friendslist table: <id>,<user>,<friend>
		// index is at 2
		ss << "SELECT * FROM users WHERE id=" << res.rowAt(i)[2];

		// resolve this username by cross-referencing it with the users table
		QueryResult resolve=query(ss.str());
		if (!resolve.error())
			friends.push_back(resolve.rowAt(0)[1]);

		ss.str("");
	}

	return friends;
}

std::string DatabaseSQLite3::compareFoldCase(const std::string &column, const std::string &str) {
	std::string sql=column;
	sql+=" like '";
	sql+=str;
	sql+="'";

	return sql;
}
