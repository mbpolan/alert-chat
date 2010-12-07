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
// database.h: declaration of the database class

#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <list>
#include <vector>

class Database {
	public:
		class QueryResult {
			public:
				typedef std::vector<std::vector<std::string> > RowTable;

			public:
				QueryResult(int r, int c, const RowTable &rowData,
						const std::vector<std::string> &columnNames);

				// constructs a null result query with an error message
				QueryResult(const std::string &errorMessage);

				std::string errorMessage() const { return m_ErrorMessage; }
				bool error() const { return !m_ErrorMessage.empty(); }

				int rowCount() { return m_NumRows; }
				int columnCount() { return m_NumCols; }

				std::string columnName(int index) const;
				std::vector<std::string> rowAt(int index) const;

			private:
				std::vector<std::string> m_ColumnNames;
				RowTable m_RowData;
				std::string m_ErrorMessage;

				int m_NumRows;
				int m_NumCols;
		};

	public:
		Database();
		
		std::string lastError();

		virtual bool open()=0;
		virtual bool close()=0;

		virtual std::list<std::string> getFriendList(const std::string &username)=0;

	protected:
		std::string m_LastError;
};

#endif
