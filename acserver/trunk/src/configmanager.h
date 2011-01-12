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

/**
 * Class responsible for loading and storing values that are written
 * in the server.conf configuration file.
 * Values are stored in instances of the Variable class, which assists in keeping the
 * internal database simple. Furthermore, it provides a simple interface for fetching
 * values from the file by letting the programmer assign a global manager instance that
 * can be accessed across the code.
 */
class ConfigManager {
	public:
		/**
		 * Creates a manager for the given file.
		 *
		 * @param file Path to the configuration file to load.
		 */
		ConfigManager(const std::string &file);

		/**
		 * Returns a pointer to the default configuration manager which was set
		 * at some point in the code.
		 *
		 * @return Pointer to the global ConfigManager, or NULL if one was never set.
		 */
		static ConfigManager* manager();

		/**
		 * Sets a given configuration manager as the default.
		 * @param cm Pointer to a ConfigManager object, or NULL to clear any previously
		 * set configuration managers.
		 */
		static void setDefaultManager(ConfigManager *cm);

		/**
		 * Attempts to parse the configuration file, who's path is pointed to by
		 * the first argument supplied to the constructor.
		 *
		 * @return True if parsing succeeded, or false otherwise.
		 */
		bool parse();

		/**
		 * Returns the value associated with the given key. If no such key exists,
		 * then an invalid Variable object is returned.
		 *
		 * @param key The key to look up.
		 *
		 * @return The value associated with said key, or null Variable object if there is no such key.
		 */
		Variable valueForKey(const std::string &key);

	private:
		/// Typedef of a map, associating string keys with Variable values.
		typedef std::map<std::string, Variable> VariableMap;

		/// The file assigned to this configuration manager.
		std::string m_File;

		/// Internal hash map where values are mapped to their corresponding keys.
		VariableMap m_Keys;
};

#endif
