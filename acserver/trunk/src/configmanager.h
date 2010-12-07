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
// configmanager.h: definitions of the ConfigManager class

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <iostream>
#include <map>

#include "definitions.h"
#include "variable.h"

typedef std::map<std::string, Variable> VariableMap;

class ConfigManager {
	public:
		ConfigManager(const std::string &file);
		~ConfigManager();

		static ConfigManager* manager();
		static void setDefaultManager(ConfigManager *cm);

		bool parse();

		Variable valueForKey(const std::string &key);

	private:
		std::string m_File;
		VariableMap m_Keys;
};

#endif
