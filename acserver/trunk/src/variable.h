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

class Variable {
	public:
		enum Type { Null=0, Integer, String };

		Variable();
		Variable(int n);
		Variable(const std::string &str);

		bool isNull() { return m_DataType==Null; }

		Type dataType() const { return m_DataType; }

		int toInt() const;
		std::string toString() const;

	private:
		int m_IntValue;
		std::string m_StringValue;

		Type m_DataType;
};

#endif
