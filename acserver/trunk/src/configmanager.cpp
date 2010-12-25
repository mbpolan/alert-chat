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
// configmanager.cpp: implementation of ConfigManager class

#include <cstdio>
#include <cstdlib>

#include "configmanager.h"

ConfigManager *g_Manager=NULL;

ConfigManager::ConfigManager(const std::string &file) {
	m_File=file;
}

ConfigManager::ConfigManager* ConfigManager::manager() {
	return g_Manager;
}

void ConfigManager::setDefaultManager(ConfigManager *cm) {
	g_Manager=cm;
}

bool ConfigManager::parse() {
	FILE *f=fopen(m_File.c_str(), "r");
	if (!f)
		return false;

	char data[256];
	while(fgets(data, 256, f)) {
		// skip commented lines
		if (data[0]=='#' || data[0]=='\n')
			continue;

		std::string line=data;

		// we expect lines in the form key=value for numerical values
		// or key="value" for strings; extract the key
		int pos=line.find('=');
		if (pos==-1) {
			std::cout << "ConfigManager: missing \'=\' in line: " << line << std::endl;
			continue;
		}

		// get our key
		std::string key=line.substr(0,pos-1);

		// find the first non space character after the equals sign
		int vstart=-1;
		for (int i=pos+1; i<line.size(); i++) {
			if (line[i]!=' ') {
				vstart=i;
				break;
			}
		}

		// make sure this is valid
		if (vstart==-1) {
			std::cout << "ConfigManager: missing value for key in line: " << line << std::endl;
			continue;
		}

		// get our value: string
		if (line[vstart]=='\"') {
			// further verification to see if there is an ending quote mark
			int vend=line.rfind("\"");
			if (vend==vstart) {
				std::cout << "ConfigManager: missing terminating \" in line: " << line << std::endl;
				continue;
			}

			std::string val=line.substr(vstart+1, vend-vstart-1);
			m_Keys[key]=Variable(val);
		}

		// get our value: integer
		else {
			int val=atoi(line.substr(vstart, line.size()-vstart).c_str());
			m_Keys[key]=Variable(val);
		}
	}

	fclose(f);
	return true;
}

Variable ConfigManager::valueForKey(const std::string &key) {
	return m_Keys[key];
}
