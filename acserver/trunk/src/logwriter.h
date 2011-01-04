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
// logwriter.h: definitions of the LogWriter class

#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <iostream>

/**
 * Provides logging facilities for the server.
 * The server provides three distinct types of logs: an access log, an error log, and
 * a master log. Each log type can be activated or deactivated in the configuration
 * file, as per the administrator's discretion. An access log contains data related to
 * when a user connects (IP address and time) and disconnects. Error logs keep track of
 * various problems that arise as the server runs. This helps to prevent an unnecessary
 * build up of messages that are sent to the terminal. Furthermore, a master log is provided
 * that basically contains ALL data that would otherwise be sent to individual logs.
 *
 * Before using any of the logging features, make sure to initialize the subsystem with a
 * call to LogWriter::initialize(), preferably as soon as possible. This will enable the server
 * to begin keeping track of its run time more efficiently.
 */
class LogWriter {
	public:
		/// Defines the type of log.
		enum Log { Access=0x1, Error=0x2, Master=0x4 };

		/**
		 * Creates a dummy log writer that sends output to the given file.
		 * Normally there should be no need to create a log writer directly, as the
		 * logging subsystems takes care of creating the appropriate writers.
		 *
		 * @param outFile The file to send output to.
		 */
		LogWriter(const std::string &outFile);

		/**
		 * Returns a writer for the given type of log.
		 *
		 * @param logType The type of log to request a writer for.
		 * @return The writer for the given log, or NULL if no writer exists.
		 */
		static LogWriter* writer(unsigned int logType);

		/**
		 * Initializes and prepares the logging subsystem.
		 * The logging subsystem will create writers for the given log types. Passing
		 * OR'd values of the Log enum for the logs parameter defines what writers
		 * to create.
		 *
		 * @param An OR'd value of the Log enum.
		 * @return True if the subsystem could be initialized, false otherwise.
		 */
		static bool initialize(unsigned int logs);

		/**
		 * Writes a line of characters to the given log.
		 * This function automatically handles writing to multiple logs, if the
		 * server configuration is set as such. For example, if a master log is also
		 * requested by the administrator, then this function will not only write to one
		 * of the two basic logs (Access or Error), but to the master log as well.
		 */
		static void output(const std::string &message, const Log &type);

		/**
		 * Writes a string to the output file.
		 *
		 * @param line String of data to write.
		 */
		void writeLine(const std::string &line);

	private:
		/// Defines the path of the output file.
		std::string m_OutFile;
};

#endif
