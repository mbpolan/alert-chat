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
#include <vector>

#include "definitions.h"

/**
 * Abstraction of a general database driver and manager.
 * The Database class provides an interface that subclassed database drivers
 * need to inherit from, in order to maintain a standard for accessing, querying,
 * and performing other database actions. By enforcing such a protocol, a pointer
 * to the Database class will be able to encapsulate whatever actual driver is being
 * used to perform work on the database.
 *
 * In order to make the process of querying a database simple, each query must store
 * its results in an instance of the QueryResult class. An SQLite3 database would, for
 * example, first parse its internal array and progressively build the row data stored
 * in this data structure. Further, the QueryResult class provides a way to report
 * errors without adding another layer of abstraction.
 */
class Database {
	public:
		/**
		 * Structure used to store data returned from a database query.
		 * This class provides a very high-level interface to access the data
		 * returned after executing an SQL query on the open database. Error reporting
		 * is handled by the error() and errorMessage() functions, which should always
		 * be checked before assuming a query succeeded.
		 */
		class QueryResult {
			public:
				/// A matrix of data for the rows and columns of a database query.
				typedef std::vector<StringVector > RowTable;

			public:
				/**
				 * Constructs a query result with the given amount of rows and columns,
				 * along with the actual table data and, optionally, the names of the columns,
				 * where supported.
				 *
				 * @param r The number of rows.
				 * @param c The number of columns.
				 * @param rowData Matrix of data representing rows in the query.
				 * @param columnNames Optionally a list of column names corresponding to the query.
				 */
				QueryResult(int r, int c, const RowTable &rowData,
						const StringVector &columnNames);

				/**
				 * Constructs a NULL query with the given error message.
				 * It is implied that a return of this kind of query object means
				 * that some sort of error occurred in the execution of a query.
				 *
				 * @param errorMessage The message to set as the error string.
				 */
				QueryResult(const std::string &errorMessage);

				/**
				 * Returns the error that occurred during query execution.
				 *
				 * @return Human readble message describing the error that occurred.
				 */
				std::string errorMessage() const { return m_ErrorMessage; }

				/**
				 * Polls the query to see if an error occurred.
				 *
				 * @return True if an error occurred, false otherwise.
				 */
				bool error() const { return !m_ErrorMessage.empty(); }

				/**
				 * Returns the number of result rows.
				 */
				int rowCount() { return m_NumRows; }

				/**
				 * Returns the number of columns in this query.
				 */
				int columnCount() { return m_NumCols; }

				/**
				 * Returns the name of the column at the given 0 based index.
				 */
				std::string columnName(int index) const;

				/**
				 * Returns the row data for the row at the given 0 based index.
				 */
				StringVector rowAt(int index) const;

			private:
				/// A vector of column names (optional).
				StringVector m_ColumnNames;

				/// Matrix of row data.
				RowTable m_RowData;

				/// Human readable error message string.
				std::string m_ErrorMessage;

				/// Number of rows in the query.
				int m_NumRows;

				/// Number of columns in the query
				int m_NumCols;
		};

	public:
		/**
		 * Creates a database handle, based on what data storage type is defined in
		 * the configuration file.
		 *
		 * @return A handle to a database.
		 */
		static Database* getHandle();

		/**
		 * Returns the last error to have occurred during database operation.
		 *
		 * @return Error string if an error happened, or a blank string otherwise.
		 */
		std::string lastError();

		/**
		 * Attempts to open the database or file, depending on the driver.
		 *
		 * @return True if the database could be accessed, false otherwise.
		 */
		virtual bool open()=0;

		/**
		 * Closes the connection or database file.
		 *
		 * @return True if the database was closed, false otherwise.
		 */
		virtual bool close()=0;

		/**
		 * Executes the provided SQL statement.
		 *
		 * @param sql The SQL statement to execute.
		 * @return Query object which stores the result.
		 */
		virtual Database::QueryResult query(const std::string &sql)=0;

		/**
		 * Maps a given username to its primary key in the database.
		 *
		 * @param username The username to look up.
		 * @return The user's primary key ID.
		 */
		virtual int getUserID(const std::string &username)=0;

		/**
		 * Extracts all the users who are on the given user's friend list.
		 *
		 * @return A vector of usernames, or empty if there are none.
		 */
		virtual StringList getFriendList(int id)=0;

		/**
		 * Extracts all the users who are on the given user's blocked list.
		 *
		 * @return A vector of usernames, or empty if there are none.
		 */
		virtual StringList getBlockedList(int id)=0;

		/**
		 * Returns a formatted SQL statement for case insensitive string comparison.
		 * This is useful for finding users in the database based strictly on their
		 * base names, regardless of how the name itself is formatted.
		 *
		 * @param column The table column name
		 * @param str The string to compare against
		 * @return A formatted SQL statement.
		 */
		virtual std::string compareFoldCase(const std::string &column, const std::string &str)=0;


	protected:
		/// Hidden constructor; initializes database members.
		Database();

		/// String containing the last error that occurred for this database.
		std::string m_LastError;
};

#endif
