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
// variable.cpp: implementation of the Variable class

#include "variable.h"

Variable::Variable() {
	m_DataType=Null;
}

Variable::Variable(int n) {
	m_DataType=Integer;
	m_IntValue=n;
}

Variable::Variable(const std::string &str) {
	m_DataType=String;
	m_StringValue=str;
}

int Variable::toInt() const {
	if (m_DataType!=Integer)
		return 0;

	return m_IntValue;
}

std::string Variable::toString() const {
	if (m_DataType!=String)
		return "";

	return m_StringValue;
}
