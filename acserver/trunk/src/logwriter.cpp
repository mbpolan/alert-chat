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
// logwriter.cpp: implementation of the LogWriter class

#include <cstdio>
#include <ctime>
#include <map>

#include "configmanager.h"
#include "logwriter.h"
#include "thread.h"

std::map<unsigned int,LogWriter*> g_Writers;

LogWriter::LogWriter(const std::string &outFile) {
	m_OutFile=outFile;
}

LogWriter* LogWriter::writer(unsigned int logType) {
	if (g_Writers.find(logType)==g_Writers.end())
		return NULL;

	return g_Writers[logType];
}

bool LogWriter::initialize(unsigned int logs) {
	if (logs & Access)
		g_Writers[Access]=new LogWriter(ConfigManager::manager()->valueForKey("log-access").toString());

	if (logs & Error)
		g_Writers[Error]=new LogWriter(ConfigManager::manager()->valueForKey("log-error").toString());

	if (logs & Master)
		g_Writers[Master]=new LogWriter(ConfigManager::manager()->valueForKey("log-master").toString());

	return true;
}

void LogWriter::output(const std::string &message, const LogWriter::Log &type) {
	lock(&Threads::g_Mutexes[MUTEX_LOGWRITER]);

	writer(type)->writeLine(message);

	// also send to master log, if it exists
	if (type!=Master && LogWriter::writer(Master))
		writer(Master)->writeLine(message);

	unlock(&Threads::g_Mutexes[MUTEX_LOGWRITER]);
}

void LogWriter::writeLine(const std::string &line) {
	FILE *f=fopen(m_OutFile.c_str(), "a");
	if (!f)
		return;

	time_t now;
	time(&now);

	// format the timestamp (ctime throws in a newline character for some reason)
	std::string t=ctime(&now);
	t=t.substr(0, t.size()-2);

	// include the time of this message
	std::string out="[";
	out+=t;
	out+="] ";
	out+=line;
	out+="\n";

	fputs(out.c_str(), f);
	fclose(f);
}
