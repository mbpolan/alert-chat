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
// historystore.cpp: implementation of the HistoryStore class

#include <QDesktopServices>
#include <QDir>
#include <QFile>

#include "historystore.h"

HistoryStore::HistoryStore(QObject *parent): QObject(parent) {
    // create our storage directory if needed
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    if (!dir.exists("Alert Chat Histories"))
	  dir.mkdir("Alert Chat Histories");
}

// adds a line of message to a user's saved history
void HistoryStore::appendTextMessage(const QString &account, const QString &username, const QString &line) {
    // make sure the account directory exists
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    dir.cd("Alert Chat Histories");
    if (!dir.exists(account))
	  dir.mkdir(account);

    // open the target file, based on lowercase username
    QString path=QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path+="/Alert Chat Histories/";
    path+=account+"/";
    path+=username.toLower();
    path+=".txt";

    QFile file(path);
    if (file.open(QIODevice::Append)) {
	  file.write(QString(line+"\n").toStdString().c_str());
	  file.close();
    }
}

HistoryStore::DataMap HistoryStore::savedHistories() {
    // saved histories are stored as text files in the user's document store location,
    // under the Alert Chat Histories directory
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    dir.cd("Alert Chat Histories");

    // first get a list of all account directories
    DataMap data;
    QStringList accounts=dir.entryList();
    for (int i=0; i<accounts.size(); i++) {
          if (accounts.at(i)=="." || accounts.at(i)=="..")
		continue;

	  // go into the directory
	  dir.cd(accounts.at(i));

	  // pull all file names, ending with .txt
	  QStringList filters;
	  filters << "*.txt";
	  QList<QString> entries=dir.entryList(filters);

	  // remove the file extensions
	  for (QList<QString>::iterator it=entries.begin(); it!=entries.end(); ++it) {
		QString &entry=(*it);
		entry=entry.left(entry.size()-4);
	  }

	  data[accounts.at(i)]=entries;
	  dir.cdUp();
    }

    return data;
}

QString HistoryStore::userHistory(const QString &account, const QString &username) {
    // open the target file, based on lowercase username
    QString path=QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path+="/Alert Chat Histories/";
    path+=account+"/";
    path+=username.toLower();
    path+=".txt";

    QString contents="";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
	  contents=file.readAll();
	  file.close();
    }

    return contents;
}
