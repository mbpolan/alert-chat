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
// database.cpp: implementation of the Database class

#include "database.h"

Database::QueryResult::QueryResult(int r, int c, const RowTable &rowData,
					     const std::vector<std::string> &columnNames) {
	m_NumRows=r;
	m_NumCols=c;
	m_ColumnNames=columnNames;
	m_RowData=rowData;
	m_ErrorMessage="";
}

Database::QueryResult::QueryResult(const std::string &error) {
	m_ErrorMessage=error;
}

std::string Database::QueryResult::columnName(int index) const {
	return m_ColumnNames[index];
}

std::vector<std::string> Database::QueryResult::rowAt(int index) const {
	return m_RowData[index];
}

/***********************************************************************/

Database::Database() {
	m_LastError="";
}

std::string Database::lastError() {
	std::string err=m_LastError;
	m_LastError="";

	return m_LastError;
}
