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
// variable.h: definitions of the Variable class

#ifndef VARIABLE_H
#define VARIABLE_H

#include <cctype>
#include <iostream>

/**
 * Object that is capable of storing various data types.
 * When multiple data types need to be stored in, say, a vector, the Variable class
 * can be used to provide a transparent way of storing said values without having to
 * use multiple containers specific to certain data types.
 *
 * The Variable class keeps track of what data type is was set to hold. Think of this
 * class as a "C++" void pointer encapsulation, only much safer.
 */
class Variable {
	public:
		/// The data type this object is storing.
		enum Type { Null=0, Integer, String };

		/**
		 * Creates a null Variable object.
		 * This is useful for error reporting. Consider a hash map of values which are of
		 * this type. If the user requests a key that's not in the map, a null Variable can
		 * be returned to signal the error condition.
		 */
		Variable();

		/**
		 * Creates a Variable object that stores an integer.
		 */
		Variable(int n);

		/**
		 * Creates a Variable object that stores a string.
		 */
		Variable(const std::string &str);

		/**
		 * Checks whether or not this Variable object contains data.
		 *
		 * @return True if NO data is in this object, false otherwise.
		 */
		bool isNull() { return m_DataType==Null; }

		/**
		 * Returns the type of data that is stored in this Variable object.
		 */
		Type dataType() const { return m_DataType; }

		/**
		 * Returns the internal data as an integer.
		 */
		int toInt() const;

		/**
		 * Returns the internal data as a string.
		 */
		std::string toString() const;

	private:
		/// An integer value.
		int m_IntValue;

		/// A string value.
		std::string m_StringValue;

		/// The type of data stored.
		Type m_DataType;
};

#endif
